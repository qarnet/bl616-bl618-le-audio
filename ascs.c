#include <sys/errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
//#include <blog.h>

#include "bluetooth.h"
#include "conn.h"
#include "conn_internal.h"
#include "gatt.h"
#include "hci_core.h"
#include "uuid.h"
#include "ascs.h"
#include "bt_log.h"

// #define TP_PRIO configMAX_PRIORITIES - 5

// static void ble_tp_connected(struct bt_conn *conn, u8_t err);
// static void ble_tp_disconnected(struct bt_conn *conn, u8_t reason);
// static int bl_tp_send_indicate(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *data, u16_t len);

// struct bt_conn *ble_tp_conn;
// struct bt_gatt_exchange_params exchg_mtu;
// TaskHandle_t ble_tp_task_h;

// int tx_mtu_size = 20;
// u8_t tp_start = 0;
// static u8_t created_tp_task = 0;
// static u8_t isRegister = 0;

// static struct bt_conn_cb ble_tp_conn_callbacks = {
// 	.connected	=   ble_tp_connected,
// 	.disconnected	=   ble_tp_disconnected,
// };

// /*************************************************************************
// NAME
//     ble_tp_tx_mtu_size
// */
// static void ble_tp_tx_mtu_size(struct bt_conn *conn, u8_t err,
// 			  struct bt_gatt_exchange_params *params)
// {
//    if(!err)
//    {
//         tx_mtu_size = bt_gatt_get_mtu(ble_tp_conn);
//         BT_WARN("ble tp echange mtu size success, mtu size: %d", tx_mtu_size);
//    }
//    else
//    {
//         BT_WARN("ble tp echange mtu size failure, err: %d", err);
//    }
// }

// /*************************************************************************
// NAME
//     ble_tp_connected
// */
// static void ble_tp_connected(struct bt_conn *conn, u8_t err)
// {
//     if(err || conn->type != BT_CONN_TYPE_LE)
//     {
//         return;
//     }

//     int tx_octets = 0x00fb;
//     int tx_time = 0x0848;
//     int ret = -1;

//     BT_INFO("%s",__func__);
//     ble_tp_conn = conn;

//     //set data length after connected.
//     ret = bt_le_set_data_len(ble_tp_conn, tx_octets, tx_time);
//     if(!ret)
//     {
//         BT_WARN("ble tp set data length success.");
//     }
//     else
//     {
//         BT_WARN("ble tp set data length failure, err: %d\n", ret);
//     }

//     //exchange mtu size after connected.
//     exchg_mtu.func = ble_tp_tx_mtu_size;
//     ret = bt_gatt_exchange_mtu(ble_tp_conn, &exchg_mtu);
//     if (!ret) {
//         BT_WARN("ble tp exchange mtu size pending.");
//     } else {
//         BT_WARN("ble tp exchange mtu size failure, err: %d", ret);
//     }
// }

// /*************************************************************************
// NAME    
//     ble_tp_disconnected
// */
// static void ble_tp_disconnected(struct bt_conn *conn, u8_t reason)
// { 
//     if(conn->type != BT_CONN_TYPE_LE)
//     {
//         return;
//     }

//     BT_INFO("%s",__func__);

//     ble_tp_conn = NULL;
// }

// /*************************************************************************
// NAME
//     ble_tp_recv_rd
// */
// static int ble_tp_recv_rd(struct bt_conn *conn,	const struct bt_gatt_attr *attr,
//                                         void *buf, u16_t len, u16_t offset)
// {
//     int size = 9;
//     char data[9] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

//     memcpy(buf, data, size);

//     return size;
// }

// /*************************************************************************
// NAME
//     ble_tp_recv_wr
// */
// static int ble_tp_recv_wr(struct bt_conn *conn, const struct bt_gatt_attr *attr,
//                                         const void *buf, u16_t len, u16_t offset, u8_t flags)
// {
//     BT_WARN("recv data len=%d, offset=%d, flag=%d", len, offset, flags);

//     if (flags & BT_GATT_WRITE_FLAG_PREPARE)
//     {
//         //Don't use prepare write data, execute write will upload data again.
//         BT_WARN("rcv prepare write request");
//         return 0;
//     }

//     if(flags & BT_GATT_WRITE_FLAG_CMD)
//     {
//         //Use write command data.
//         BT_WARN("rcv write command");
//     }
//     else
//     {
//         //Use write request / execute write data.
//         BT_WARN("rcv write request / exce write");
//     }

//     return len;
// }

// /*************************************************************************
// NAME    
//     indicate_rsp /bl_tp_send_indicate
// */ 
// void indicate_rsp(struct bt_conn *conn, const struct bt_gatt_attr *attr,	u8_t err)
// {
//     BT_WARN("receive confirm, err:%d", err);
// }

// /*************************************************************************
// NAME
//     ble_tp_ind_ccc_changed
// */
// static void ble_tp_ind_ccc_changed(const struct bt_gatt_attr *attr, u16_t value)
// {
//     int err = -1;
//     char data[9] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

//     if(value == BT_GATT_CCC_INDICATE) {
//         err = bl_tp_send_indicate(ble_tp_conn, get_attr(BT_CHAR_BLE_TP_IND_ATTR_VAL_INDEX), data, 9);
//         BT_WARN("ble tp send indatcate: %d", err);
//     }
// }

// /*************************************************************************
// NAME
//     ble_tp_notify
// */
// static void ble_tp_notify_task(void *pvParameters)
// {
//     int err = -1;
//     char data[244] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

//     while(1)
//     {
//         err = bt_gatt_notify(ble_tp_conn, get_attr(BT_CHAR_BLE_TP_NOT_ATTR_VAL_INDEX), data, (tx_mtu_size - 3));
//         BT_WARN("ble tp send notify : %d", err);
//     }
// }

// /*************************************************************************
// NAME    
//     ble_tp_not_ccc_changed
// */ 
// static void ble_tp_not_ccc_changed(const struct bt_gatt_attr *attr, u16_t value)
// {
//     int err;
//     BT_WARN("ccc:value=[%d]",value);
    
//     if(tp_start)
//     {
//         if(value == BT_GATT_CCC_NOTIFY)
//         {
//             if(xTaskCreate(ble_tp_notify_task, (char*)"bletp", 256, NULL, TP_PRIO, &ble_tp_task_h) == pdPASS)
//             {
//                 created_tp_task = 1;
//                 BT_WARN("Create throughput tx task success.");
//             }       
//             else        
//             {      
//                 created_tp_task = 0;
//                 BT_WARN("Create throughput tx task fail.");
//             }
//         }
//         else
//         {
//             if(created_tp_task)
//             {
//                 BT_WARN("Delete throughput tx task.");
//                 vTaskDelete(ble_tp_task_h);
//                 created_tp_task = 0;
//             }
//         }
//     }
//     else
//     {
//         if(created_tp_task)
//         {
//             BT_WARN("Delete throughput tx task.");
//             vTaskDelete(ble_tp_task_h);
//             created_tp_task = 0;
//         }
//         if(value == BT_GATT_CCC_NOTIFY) {
//             err = bt_gatt_notify(ble_tp_conn, get_attr(BT_CHAR_BLE_TP_NOT_ATTR_VAL_INDEX), "notify", strlen("notify"));
//             BT_WARN("ble tp send indatcate: %d", err);
//         }
//     }
// }

// /*************************************************************************
// *  DEFINE : attrs 
// */
// static struct bt_gatt_attr attrs[]= {
// 	BT_GATT_PRIMARY_SERVICE(BT_UUID_SVC_BLE_TP),

//         BT_GATT_CHARACTERISTIC(BT_UUID_CHAR_BLE_TP_RD,
// 							BT_GATT_CHRC_READ,
// 							BT_GATT_PERM_READ,
// 							ble_tp_recv_rd,
// 							NULL,
// 							NULL),

// 	BT_GATT_CHARACTERISTIC(BT_UUID_CHAR_BLE_TP_WR,
// 							BT_GATT_CHRC_WRITE |BT_GATT_CHRC_WRITE_WITHOUT_RESP,
// 							BT_GATT_PERM_WRITE|BT_GATT_PERM_PREPARE_WRITE,
// 							NULL,
// 							ble_tp_recv_wr,
// 							NULL),

// 	BT_GATT_CHARACTERISTIC(BT_UUID_CHAR_BLE_TP_IND,
// 							BT_GATT_CHRC_INDICATE,
// 							0,
// 							NULL,
// 							NULL,
// 							NULL),

// 	BT_GATT_CCC(ble_tp_ind_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

// 	BT_GATT_CHARACTERISTIC(BT_UUID_CHAR_BLE_TP_NOT,
// 							BT_GATT_CHRC_NOTIFY,
// 							0,
// 							NULL,
// 							NULL,
// 							NULL),
							
// 	BT_GATT_CCC(ble_tp_not_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE)

// };

// static int bl_tp_send_indicate(struct bt_conn *conn, const struct bt_gatt_attr *attr,
// 				                    const void *data, u16_t len)
// {
//     //indicate paramete must be allocated statically
//     static struct bt_gatt_indicate_params ind_params;
//     ind_params.attr = attr;
//     ind_params.data = data;
//     ind_params.len = len;
//     ind_params.func = indicate_rsp;
//     ind_params.uuid = NULL;
//     /*it is possible to indicate by UUID by setting it on the
//       parameters, when using this method the attribute given is used as the
//       start range when looking up for possible matches.In this case,set uuid as follows.*/
//     //ind_params->uuid = attrs[6].uuid;

//     return bt_gatt_indicate(conn, &ind_params);
// }

// /*************************************************************************
// NAME    
//     get_attr
// */
// struct bt_gatt_attr *get_attr(u8_t index)
// {
// 	return &attrs[index];
// }


// struct bt_gatt_service ble_us_pacs_server = BT_GATT_SERVICE(attrs);

typedef enum {
    ASCS_SM_STATE_IDLE = 0x00,
    ASCS_SM_STATE_CODEC_CONFIGURED = 0x01,
    ASCS_SM_STATE_QOS_CONFIGURED = 0x02,
    ASCS_SM_STATE_ENABLING = 0x03,
    ASCS_SM_STATE_STREAMING = 0x04,
    ASCS_SM_STATE_DISABLING = 0x05,
    ASCS_SM_STATE_RELEASING = 0x06,
    ASCS_SM_STATE_LAST_ENTRY,
} ascs_sm_state;

typedef enum {
    ASCS_SM_EVT_CONFIG_CODEC,
    ASCS_SM_EVT_CONFIG_QOS,
    ASCS_SM_EVT_RELEASED_CACHING,
    ASCS_SM_EVT_RELEASED_NO_CACHING,
    ASCS_SM_EVT_RELEASE,
    ASCS_SM_EVT_ENABLE,
    ASCS_SM_EVT_DISABLE,
    ASCS_SM_EVT_UPDATE_METADATA,
    ASCS_SM_EVT_RECEIVER_START_READY,
    ASCS_SM_EVT_LAST_ENTRY,
} ascs_sm_evt;

typedef ascs_sm_state (*const ascsArrayEventHandler[ASCS_SM_STATE_LAST_ENTRY][ASCS_SM_EVT_LAST_ENTRY])(void *_data);

typedef ascs_sm_state (*ascsEventHandler)(void *_data);

ascs_sm_state idleSConfigCodecEHandler(void *_data)
{
    return ASCS_SM_STATE_CODEC_CONFIGURED;
}

ascs_sm_state codecConfiguredSConfigCodecEHandler(void *_data)
{
    return ASCS_SM_STATE_CODEC_CONFIGURED;
}

ascs_sm_state codecConfiguredSReleaseEHandler(void *_data)
{
    return ASCS_SM_STATE_RELEASING;
}

ascs_sm_state codecConfiguredSconfigQoSEHandler(void *_data)
{
    return ASCS_SM_STATE_QOS_CONFIGURED;
}

ascs_sm_state qoSConfiguredSConfigCodecEHandler(void *_data)
{
    return ASCS_SM_STATE_CODEC_CONFIGURED;
}

ascs_sm_state qoSConfiguredSConfigQoSEHandler(void *_data)
{
    return ASCS_SM_STATE_QOS_CONFIGURED;
}

ascs_sm_state qoSConfiguredSReleaseEHandler(void *_data)
{
    return ASCS_SM_STATE_RELEASING;
}

ascs_sm_state qoSConfiguredSEnableEHandler(void *_data)
{
    return ASCS_SM_STATE_ENABLING;
}

ascs_sm_state enablingSReleaseEHandler(void *_data)
{
    return ASCS_SM_STATE_RELEASING;
}

ascs_sm_state enablingSUpdateMetadataEHandler(void *_data)
{
    return ASCS_SM_STATE_ENABLING;
}

ascs_sm_state enablingSDisableEHandler(void *_data)
{
    return ASCS_SM_STATE_QOS_CONFIGURED;
}

ascs_sm_state enablingSReceiverStartReadyEHandler(void *_data)
{
    return ASCS_SM_STATE_STREAMING;
}

ascs_sm_state streamingSUpdateMetadataEHandler(void *_data)
{
    return ASCS_SM_STATE_STREAMING;
}

ascs_sm_state streamingSDisableEHandler(void *_data)
{
    return ASCS_SM_STATE_QOS_CONFIGURED;
}

ascs_sm_state streamingSReleaseEHandler(void *_data)
{
    return ASCS_SM_STATE_RELEASING;
}

ascs_sm_state releasingSReleasedNoCachingEHandler(void *_data)
{
    return ASCS_SM_STATE_IDLE;
}

ascs_sm_state releasingSReleasedCachingEHandler(void *_data)
{
    return ASCS_SM_STATE_CODEC_CONFIGURED;
}



void ascs_init()
{
    printf("ASCS Init\n");

    ascs_sm_state current_state = ASCS_SM_STATE_IDLE;
    ascs_sm_evt new_event;

    static ascsArrayEventHandler stateMachine = {
        [ASCS_SM_STATE_IDLE] = {[ASCS_SM_EVT_CONFIG_CODEC] = idleSConfigCodecEHandler},
        [ASCS_SM_STATE_CODEC_CONFIGURED] = {[ASCS_SM_EVT_CONFIG_CODEC] = codecConfiguredSConfigCodecEHandler},
        [ASCS_SM_STATE_CODEC_CONFIGURED] = {[ASCS_SM_EVT_RELEASE] = codecConfiguredSReleaseEHandler},
        [ASCS_SM_STATE_CODEC_CONFIGURED] = {[ASCS_SM_EVT_CONFIG_QOS] = codecConfiguredSconfigQoSEHandler},
        [ASCS_SM_STATE_QOS_CONFIGURED] = {[ASCS_SM_EVT_CONFIG_CODEC] = qoSConfiguredSConfigCodecEHandler},
        [ASCS_SM_STATE_QOS_CONFIGURED] = {[ASCS_SM_EVT_CONFIG_QOS] = qoSConfiguredSConfigQoSEHandler},
        [ASCS_SM_STATE_QOS_CONFIGURED] = {[ASCS_SM_EVT_RELEASE] = qoSConfiguredSReleaseEHandler},
        [ASCS_SM_STATE_QOS_CONFIGURED] = {[ASCS_SM_EVT_ENABLE] = qoSConfiguredSEnableEHandler},
        [ASCS_SM_STATE_ENABLING] = {[ASCS_SM_EVT_RELEASE] = enablingSReleaseEHandler},
        [ASCS_SM_STATE_ENABLING] = {[ASCS_SM_EVT_UPDATE_METADATA] = enablingSUpdateMetadataEHandler},
        [ASCS_SM_STATE_ENABLING] = {[ASCS_SM_EVT_DISABLE] = enablingSDisableEHandler},
        [ASCS_SM_STATE_ENABLING] = {[ASCS_SM_EVT_RECEIVER_START_READY] = enablingSReceiverStartReadyEHandler},
        [ASCS_SM_STATE_STREAMING] = {[ASCS_SM_EVT_UPDATE_METADATA] = streamingSUpdateMetadataEHandler},
        [ASCS_SM_STATE_STREAMING] = {[ASCS_SM_EVT_DISABLE] = streamingSDisableEHandler},
        [ASCS_SM_STATE_STREAMING] = {[ASCS_SM_EVT_RELEASE] = streamingSReleaseEHandler},
        [ASCS_SM_STATE_RELEASING] = {[ASCS_SM_EVT_RELEASED_NO_CACHING] = releasingSReleasedNoCachingEHandler},
        [ASCS_SM_STATE_RELEASING] = {[ASCS_SM_EVT_RELEASED_CACHING] = releasingSReleasedCachingEHandler},
    };

    return;

    // if( !isRegister )
    // {
    //     isRegister = 1;
    //     bt_conn_cb_register(&ble_tp_conn_callbacks);
    //     bt_gatt_service_register(&ble_us_pacs_server);
    // }
}



