#include "pacs.h"

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
#include "bt_log.h"

#define TP_PRIO configMAX_PRIORITIES - 5

static void ble_pacs_connected(struct bt_conn *conn, u8_t err);
static void ble_pacs_disconnected(struct bt_conn *conn, u8_t reason);

static struct bt_conn *ble_pacs_conn;
static struct bt_gatt_exchange_params exchg_mtu;

static int tx_mtu_size = 20;
static u8_t isRegister = 0;

static struct bt_conn_cb ble_pacs_conn_callbacks = {
	.connected	=   ble_pacs_connected,
	.disconnected	=   ble_pacs_disconnected,
};

static struct bt_gatt_attr *get_attr(u8_t index);

static void ble_pacs_tx_mtu_size(struct bt_conn *conn, u8_t err,
			  struct bt_gatt_exchange_params *params)
{
   if(!err)
   {
        tx_mtu_size = bt_gatt_get_mtu(ble_pacs_conn);
        BT_WARN("ble tp echange mtu size success, mtu size: %d", tx_mtu_size);
   }
   else
   {
        BT_WARN("ble tp echange mtu size failure, err: %d", err);
   }
}

static void ble_pacs_connected(struct bt_conn *conn, u8_t err)
{
    if(err || conn->type != BT_CONN_TYPE_LE)
    {
        return;
    }

    int tx_octets = 0x00fb;
    int tx_time = 0x0848;
    int ret = -1;

    BT_INFO("%s",__func__);
    ble_pacs_conn = conn;

    //set data length after connected.
    ret = bt_le_set_data_len(ble_pacs_conn, tx_octets, tx_time);
    if(!ret)
    {
        BT_WARN("ble tp set data length success.");
    }
    else
    {
        BT_WARN("ble tp set data length failure, err: %d\n", ret);
    }

    //exchange mtu size after connected.
    exchg_mtu.func = ble_pacs_tx_mtu_size;
    ret = bt_gatt_exchange_mtu(ble_pacs_conn, &exchg_mtu);
    if (!ret) {
        BT_WARN("ble tp exchange mtu size pending.");
    } else {
        BT_WARN("ble tp exchange mtu size failure, err: %d", ret);
    }
}

static void ble_pacs_disconnected(struct bt_conn *conn, u8_t reason)
{ 
    if(conn->type != BT_CONN_TYPE_LE)
    {
        return;
    }

    BT_INFO("%s",__func__);

    ble_pacs_conn = NULL;
}

/*************************************************************************
*  DATA
*************************************************************************/

static uint8_t pac_record[] = {
    // Number_of_PAC_records
    0x01,

    // Codec_ID[0]
    0x06, // Coding_Format = LC3
    0x0000, // Company_ID
    0x0000, // Vendor-specific
    // Codec_Specific_Capabilities_Length[0]
    0x03,
    // Codec_Specific_Capabilities[0] - Supported_Sampling_Frequencies
    0x01,
    ((BIT(15) | BIT(4) | BIT(2)) >> 8), 
    (uint8_t)(BIT(15) | BIT(4) | BIT(2)),
    // Metadata_Length[0]
    0x00,
    // Metadata[0]
    // 0x00,
    // Codec_Specific_Capabilities[1] - Supported-Frame-Durations
    0x02,
    BIT(1),
    // Metadata_Length[1]
    0x00,
    // Metadata[1]
    // 0x00,
    // Codec_Specific_Capabilities[2] - Supported_Octets_Per_Codec_Frame
    0x04,
    0x28,
    0x3c,
    // Metadata_Length[2]
    0x00,
    // Metadata[2]
    // 0x00,
};

/*************************************************************************
*  Reads
*************************************************************************/

static int ble_sink_pac_recv_rd(struct bt_conn *conn,	const struct bt_gatt_attr *attr,
                                        void *buf, u16_t len, u16_t offset)
{
    int pac_record_size = sizeof(pac_record);
    memcpy(buf, pac_record, pac_record_size);

    return pac_record_size;
}

static int ble_sink_audio_locations_recv_rd(struct bt_conn *conn,	const struct bt_gatt_attr *attr,
                                        void *buf, u16_t len, u16_t offset)
{
    int size = 9;
    char data[9] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

    memcpy(buf, data, size);

    return size;
}


static int ble_available_audio_contexts_recv_rd(struct bt_conn *conn,	const struct bt_gatt_attr *attr,
                                        void *buf, u16_t len, u16_t offset)
{
    int size = 9;
    char data[9] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

    memcpy(buf, data, size);

    return size;
}


static int ble_supported_audio_contexts_recv_rd(struct bt_conn *conn,	const struct bt_gatt_attr *attr,
                                        void *buf, u16_t len, u16_t offset)
{
    int size = 9;
    char data[9] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

    memcpy(buf, data, size);

    return size;
}

/*************************************************************************
*  Writes 
*************************************************************************/

static int ble_sink_audio_locations_recv_wr(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                                        const void *buf, u16_t len, u16_t offset, u8_t flags)
{
    BT_WARN("recv data len=%d, offset=%d, flag=%d", len, offset, flags);

    if (flags & BT_GATT_WRITE_FLAG_PREPARE)
    {
        //Don't use prepare write data, execute write will upload data again.
        BT_WARN("rcv prepare write request");
        return 0;
    }

    if(flags & BT_GATT_WRITE_FLAG_CMD)
    {
        //Use write command data.
        BT_WARN("rcv write command");
    }
    else
    {
        //Use write request / execute write data.
        BT_WARN("rcv write request / exce write");
    }

    return len;
}

/*************************************************************************
*  Notifies
*************************************************************************/

static void ble_sink_pac_notify_ccc_changed(const struct bt_gatt_attr *attr, u16_t value)
{
    int err = -1;
    char data[9] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

    if(value == BT_GATT_CCC_INDICATE) { // If might not be needed
        err = bt_gatt_notify(ble_pacs_conn, get_attr(BT_CHRC_SINK_PACK_NOTIFY_ATTR_VAL_INDEX), data, (tx_mtu_size - 3));
        BT_WARN("ble tp send indatcate: %d", err);
    }
}

static void ble_sink_audio_locations_notify_ccc_changed(const struct bt_gatt_attr *attr, u16_t value)
{
    int err = -1;
    char data[9] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

    if(value == BT_GATT_CCC_INDICATE) { // If might not be needed
        err = bt_gatt_notify(ble_pacs_conn, get_attr(BT_CHRC_SINK_AUDIO_LOCATIONS_NOTIFY_ATTR_VAL_INDEX), data, (tx_mtu_size - 3));
        BT_WARN("ble tp send indatcate: %d", err);
    }
}

static void ble_available_audio_contexts_notify_ccc_changed(const struct bt_gatt_attr *attr, u16_t value)
{
    int err = -1;
    char data[9] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

    if(value == BT_GATT_CCC_INDICATE) { // If might not be needed
        err = bt_gatt_notify(ble_pacs_conn, get_attr(BT_CHRC_SINK_AUDIO_LOCATIONS_NOTIFY_ATTR_VAL_INDEX), data, (tx_mtu_size - 3));
        BT_WARN("ble tp send indatcate: %d", err);
    }
}

static void ble_supported_audio_contexts_notify_ccc_changed(const struct bt_gatt_attr *attr, u16_t value)
{
    int err = -1;
    char data[9] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

    if(value == BT_GATT_CCC_INDICATE) { // If might not be needed
        err = bt_gatt_notify(ble_pacs_conn, get_attr(BT_CHRC_SUPPORTED_AUDIO_CONTEXTS_NOTIFY_ATTR_VAL_INDEX), data, (tx_mtu_size - 3));
        BT_WARN("ble tp send indatcate: %d", err);
    }
}

/*************************************************************************
*  DEFINE : attrs 
*************************************************************************/

static struct bt_gatt_attr attrs[]= {
	BT_GATT_PRIMARY_SERVICE(BT_UUID_SVC_BLE_PACS),

    BT_GATT_CHARACTERISTIC(BT_UUID_CHRC_SINK_PAC,
							BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY /*Optional*/,
							BT_GATT_PERM_READ_ENCRYPT,
							ble_sink_pac_recv_rd,
							NULL,
							NULL),
    BT_GATT_CCC(ble_sink_pac_notify_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

	BT_GATT_CHARACTERISTIC(BT_UUID_CHRC_SINK_AUDIO_LOCATIONS,
							BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY /*Optional*/ | BT_GATT_CHRC_WRITE /*Optional*/,
							BT_GATT_PERM_READ_ENCRYPT | BT_GATT_PERM_WRITE_ENCRYPT,
							ble_sink_audio_locations_recv_rd,
							ble_sink_audio_locations_recv_wr,
							NULL),
    BT_GATT_CCC(ble_sink_audio_locations_notify_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

	BT_GATT_CHARACTERISTIC(BT_UUID_CHRC_AVAILABLE_AUDIO_CONTEXTS,
							BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
							BT_GATT_PERM_READ_ENCRYPT,
							ble_available_audio_contexts_recv_rd,
							NULL,
							NULL),
    BT_GATT_CCC(ble_available_audio_contexts_notify_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

	BT_GATT_CHARACTERISTIC(BT_UUID_CHRC_SUPPORTED_AUDIO_CONTEXTS,
							BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY /*Optional*/,
							BT_GATT_PERM_READ_ENCRYPT,
							ble_supported_audio_contexts_recv_rd,
							NULL,
							NULL),
    BT_GATT_CCC(ble_supported_audio_contexts_notify_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
};

struct bt_gatt_service ble_pacs_server = BT_GATT_SERVICE(attrs);

static struct bt_gatt_attr *get_attr(u8_t index)
{
	return &attrs[index];
}


int pacs_init()
{
    printf("%s\r\n", "US_PACS");

    if( !isRegister )
    {
        isRegister = 1;
        bt_conn_cb_register(&ble_pacs_conn_callbacks);
        bt_gatt_service_register(&ble_pacs_server);
    }

    return 0;
}