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
#include "pascDataTypes.h"
#include "audioDataTypes.h"
#include "buf.h"
#include "audio.h"
#include "definesConfig.h"

#define TP_PRIO configMAX_PRIORITIES - 5

static void ble_pacs_connected(struct bt_conn *conn, u8_t err);
static void ble_pacs_disconnected(struct bt_conn *conn, u8_t reason);

static struct bt_conn *ble_pacs_conn;
static struct bt_gatt_exchange_params exchg_mtu;

static int tx_mtu_size = 20;
static u8_t isRegister = 0;

static volatile bool avail_audio_cntxt_is_subbed = false;

// DEFINE_SINK_PAC_CHRC_VALUE(sink_pac_val);
DEFINE_SINK_AUDIO_LOCATIONS_CHRC_VALUE(sink_audio_loc_val);
DEFINE_AVAILABLE_AUDIO_CONTEXTS_CHRC_VALUE(avail_audio_cntxt_val);
DEFINE_SUPPORTED_AUDIO_CONTEXTS_CHRC_VALUE(supp_audio_cntxt_val);

/**
 * Very bad way to define le_audio pac record
*/
codec_specific_capabilities_supported_sampling_frequencies csc_one = {
    .length = 0x03,
    .type = 0x01,
    .value =  { (BIT(2) | BIT(4)), 0 }
};
codec_specific_capabilities_supported_frame_durations csc_two = {
    .length = 0x02,
    .type = 0x02,
    .value = { BIT(1) }
};
codec_specific_capabilities_supported_octets_per_codec_frame csc_three = {
    .length = 0x05,
    .type = 0x04,
    .value = { 0x28, 0, 0x3c, 0 }
};
uint8_t lc3_codec_id[] = { 0x06, 0, 0, 0, 0 };
sink_pac_chrc_value sink_pac_val;

/**
 * BT CONN
*/

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

    // int tx_octets = 0x00fb;
    // int tx_time = 0x0848;
    // int ret = -1;

    // BT_INFO("%s",__func__);
    // ble_pacs_conn = conn;

    // //set data length after connected.
    // ret = bt_le_set_data_len(ble_pacs_conn, tx_octets, tx_time);
    // if(!ret)
    // {
    //     BT_WARN("ble tp set data length success.");
    // }
    // else
    // {
    //     BT_WARN("ble tp set data length failure, err: %d\n", ret);
    // }

    // //exchange mtu size after connected.
    // exchg_mtu.func = ble_pacs_tx_mtu_size;
    // ret = bt_gatt_exchange_mtu(ble_pacs_conn, &exchg_mtu);
    // if (!ret) {
    //     BT_WARN("ble tp exchange mtu size pending.");
    // } else {
    //     BT_WARN("ble tp exchange mtu size failure, err: %d", ret);
    // }
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
*  Reads
*************************************************************************/

NET_BUF_SIMPLE_DEFINE(sink_pac_read_buf, 256);
NET_BUF_SIMPLE_DEFINE(sink_audio_loc_read_buf, 256);
NET_BUF_SIMPLE_DEFINE(avail_audio_cntxt_read_buf, 256);
NET_BUF_SIMPLE_DEFINE(supp_audio_cntxt_read_buf, 256);

static int ble_sink_pac_recv_rd(struct bt_conn *_conn,	const struct bt_gatt_attr *_attr,
                                        void *_buf, u16_t _len, u16_t _offset)
{
    int len;

    net_buf_simple_init(&sink_pac_read_buf, 0);

    ADD_U8_TO_BUF(&sink_pac_read_buf, sink_pac_val.number_of_pac_records);

    for(int i = 0; i < sink_pac_val.number_of_pac_records; i++)
    {
        net_buf_simple_add_mem(&sink_pac_read_buf, sink_pac_val.data[i].codec_id, sizeof(sink_pac_val.data[i].codec_id));

        uint8_t csc_extract[sink_pac_val.data[i].codec_specific_capabilities_length];
        net_buf_simple_add_u8(&sink_pac_read_buf, sink_pac_val.data[i].codec_specific_capabilities_length);
        if(sink_pac_val.data[i].codec_specific_capabilities_length > 0)
        {
            memcpy(csc_extract, sink_pac_val.data[i].codec_specific_capabilities, sink_pac_val.data[i].codec_specific_capabilities_length);
            extractCodecSpecificCapabilities(csc_extract, &sink_pac_read_buf); //TODO: This extract doesn't work correctly
        }
        
        uint8_t metadata_extract[sink_pac_val.data[i].metadata_length];
        net_buf_simple_add_u8(&sink_pac_read_buf, sink_pac_val.data[i].metadata_length);
        if(sink_pac_val.data[i].metadata > 0)
        {
            memcpy(metadata_extract, sink_pac_val.data[i].metadata, sink_pac_val.data[i].metadata_length);
            extractMetadata(metadata_extract, &sink_pac_read_buf);
        }
    }

    len = sink_pac_read_buf.len;
    memcpy(_buf, sink_pac_read_buf.data, sink_pac_read_buf.len);

    net_buf_simple_reset(&sink_pac_read_buf);

    return len;
}

static int ble_sink_audio_locations_recv_rd(struct bt_conn *_conn,	const struct bt_gatt_attr *_attr,
                                        void *_buf, u16_t _len, u16_t _offset)
{
    int len;

    net_buf_simple_init(&sink_audio_loc_read_buf, 0);

    net_buf_simple_add_mem(&sink_audio_loc_read_buf, sink_audio_loc_val.sink_audio_locations, sizeof(sink_audio_loc_val.sink_audio_locations));

    len = sink_audio_loc_read_buf.len;
    memcpy(_buf, sink_audio_loc_read_buf.data, sink_audio_loc_read_buf.len);

    net_buf_simple_reset(&sink_audio_loc_read_buf);

    return len;
}


static int ble_available_audio_contexts_recv_rd(struct bt_conn *_conn,	const struct bt_gatt_attr *_attr,
                                        void *_buf, u16_t _len, u16_t _offset)
{
    int len;

    net_buf_simple_init(&avail_audio_cntxt_read_buf, 0);

    net_buf_simple_add_mem(&avail_audio_cntxt_read_buf, avail_audio_cntxt_val.available_sink_contexts, sizeof(avail_audio_cntxt_val.available_sink_contexts));
    net_buf_simple_add_mem(&avail_audio_cntxt_read_buf, avail_audio_cntxt_val.available_source_contexts, sizeof(avail_audio_cntxt_val.available_source_contexts));

    len = avail_audio_cntxt_read_buf.len;
    memcpy(_buf, avail_audio_cntxt_read_buf.data, avail_audio_cntxt_read_buf.len);

    net_buf_simple_reset(&avail_audio_cntxt_read_buf);

    return len;
}


static int ble_supported_audio_contexts_recv_rd(struct bt_conn *_conn,	const struct bt_gatt_attr *_attr,
                                        void *_buf, u16_t _len, u16_t _offset)
{
    int len;

    net_buf_simple_init(&supp_audio_cntxt_read_buf, 0);

    net_buf_simple_add_mem(&supp_audio_cntxt_read_buf, supp_audio_cntxt_val.supported_sink_contexts, sizeof(supp_audio_cntxt_val.supported_sink_contexts));
    net_buf_simple_add_mem(&supp_audio_cntxt_read_buf, supp_audio_cntxt_val.supported_source_contexts, sizeof(supp_audio_cntxt_val.supported_source_contexts));

    len = supp_audio_cntxt_read_buf.len;
    memcpy(_buf, supp_audio_cntxt_read_buf.data, supp_audio_cntxt_read_buf.len);

    net_buf_simple_reset(&supp_audio_cntxt_read_buf);

    return len;
}

/*************************************************************************
*  Writes 
*************************************************************************/

#if (PACS_OPT_FUNCTIONS_IS_ENABLED)
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
#endif

/*************************************************************************
*  Notifies
*************************************************************************/

#if (PACS_OPT_FUNCTIONS_IS_ENABLED)
static void ble_sink_pac_notify_ccc_changed(const struct bt_gatt_attr *attr, u16_t value)
{
    int err = -1;
    char data[9] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

    if(value == BT_GATT_CCC_INDICATE) { // If might not be needed
        err = bt_gatt_notify(ble_pacs_conn, get_attr(BT_CHRC_SINK_PACK_NOTIFY_ATTR_VAL_INDEX), data, (tx_mtu_size - 3));
        BT_WARN("ble tp send indatcate: %d", err);
    }
}
#endif

#if (PACS_OPT_FUNCTIONS_IS_ENABLED)
static void ble_sink_audio_locations_notify_ccc_changed(const struct bt_gatt_attr *attr, u16_t value)
{
    int err = -1;
    char data[9] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

    if(value == BT_GATT_CCC_INDICATE) { // If might not be needed
        err = bt_gatt_notify(ble_pacs_conn, get_attr(BT_CHRC_SINK_AUDIO_LOCATIONS_NOTIFY_ATTR_VAL_INDEX), data, (tx_mtu_size - 3));
        BT_WARN("ble tp send indatcate: %d", err);
    }
}
#endif

static void ble_available_audio_contexts_notify_ccc_changed(const struct bt_gatt_attr *attr, u16_t value)
{
    if(value == BT_GATT_CCC_NOTIFY)
    {
        avail_audio_cntxt_is_subbed = true;
    }

    printf("CCC VALUE TO %d\r\n", value);
}

#if (PACS_OPT_FUNCTIONS_IS_ENABLED)
static void ble_supported_audio_contexts_notify_ccc_changed(const struct bt_gatt_attr *attr, u16_t value)
{
    int err = -1;
    char data[9] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

    if(value == BT_GATT_CCC_INDICATE) { // If might not be needed
        err = bt_gatt_notify(ble_pacs_conn, get_attr(BT_CHRC_SUPPORTED_AUDIO_CONTEXTS_NOTIFY_ATTR_VAL_INDEX), data, (tx_mtu_size - 3));
        BT_WARN("ble tp send indatcate: %d", err);
    }
}
#endif

/*************************************************************************
*  DEFINE : attrs 
*************************************************************************/

static struct bt_gatt_attr attrs[]= {
	BT_GATT_PRIMARY_SERVICE(BT_UUID_SVC_BLE_PACS),

    BT_GATT_CHARACTERISTIC(BT_UUID_CHRC_SINK_PAC,
#if (PACS_OPT_FUNCTIONS_IS_ENABLED)
							BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY /*Optional*/,
#else
                            BT_GATT_CHRC_READ,
#endif                            
							BT_GATT_PERM_READ_ENCRYPT,
							ble_sink_pac_recv_rd,
							NULL,
							NULL),
#if (PACS_OPT_FUNCTIONS_IS_ENABLED)
    BT_GATT_CCC(ble_sink_pac_notify_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
#endif

	BT_GATT_CHARACTERISTIC(BT_UUID_CHRC_SINK_AUDIO_LOCATIONS,
#if (PACS_OPT_FUNCTIONS_IS_ENABLED)
							BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY /*Optional*/ | BT_GATT_CHRC_WRITE /*Optional*/,
#else
                            BT_GATT_CHRC_READ,
#endif
#if (PACS_OPT_FUNCTIONS_IS_ENABLED)
							BT_GATT_PERM_READ_ENCRYPT | BT_GATT_PERM_WRITE_ENCRYPT,
#else
                            BT_GATT_PERM_READ_ENCRYPT,
#endif
							ble_sink_audio_locations_recv_rd,
#if (PACS_OPT_FUNCTIONS_IS_ENABLED)
                            ble_sink_audio_locations_recv_wr,
#else
                            NULL,
#endif
							NULL),
#if (PACS_OPT_FUNCTIONS_IS_ENABLED)
    BT_GATT_CCC(ble_sink_audio_locations_notify_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
#endif

	BT_GATT_CHARACTERISTIC(BT_UUID_CHRC_AVAILABLE_AUDIO_CONTEXTS,
							BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
							BT_GATT_PERM_READ_ENCRYPT,
							ble_available_audio_contexts_recv_rd,
							NULL,
							NULL),
    BT_GATT_CCC(ble_available_audio_contexts_notify_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

	BT_GATT_CHARACTERISTIC(BT_UUID_CHRC_SUPPORTED_AUDIO_CONTEXTS,
#if (PACS_OPT_FUNCTIONS_IS_ENABLED)
							BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY /*Optional*/,
#else
                            BT_GATT_CHRC_READ,
#endif
							BT_GATT_PERM_READ_ENCRYPT,
							ble_supported_audio_contexts_recv_rd,
							NULL,
							NULL),
#if (PACS_OPT_FUNCTIONS_IS_ENABLED)
    BT_GATT_CCC(ble_supported_audio_contexts_notify_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
#endif
};

struct bt_gatt_service ble_pacs_server = BT_GATT_SERVICE(attrs);

static struct bt_gatt_attr *get_attr(u8_t index)
{
	return &attrs[index];
}

/**
 * GETTER/SETTER
*/

int setAvailableAudioContexts(uint16_t _available_sink_contexts, uint16_t _available_source_contexts)
{
    int err;

    struct net_buf_simple buffer;

    memcpy(avail_audio_cntxt_val.available_sink_contexts, &_available_sink_contexts, sizeof(avail_audio_cntxt_val.available_sink_contexts));
    memcpy(avail_audio_cntxt_val.available_source_contexts, &_available_source_contexts, sizeof(avail_audio_cntxt_val.available_source_contexts));

    if(avail_audio_cntxt_is_subbed) {
        net_buf_simple_init(&buffer, 8);

        net_buf_simple_add_mem(&buffer, avail_audio_cntxt_val.available_sink_contexts, sizeof(avail_audio_cntxt_val.available_sink_contexts));
        net_buf_simple_add_mem(&buffer, avail_audio_cntxt_val.available_source_contexts, sizeof(avail_audio_cntxt_val.available_source_contexts));

        err = bt_gatt_notify(ble_pacs_conn, get_attr(BT_CHRC_SINK_AUDIO_LOCATIONS_NOTIFY_ATTR_VAL_INDEX), buffer.data, buffer.len);
        BT_WARN("ble tp send indatcate: %d", err);

        net_buf_simple_reset(&avail_audio_cntxt_read_buf);
    }

    return 0;
}

int pacs_init()
{
    printf("%s\r\n", "US_PACS");
    
    sink_pac_val.number_of_pac_records = 3;
    memcpy(sink_pac_val.data[0].codec_id, lc3_codec_id, sizeof(sink_pac_val.data[0].codec_id));
    sink_pac_val.data[0].codec_specific_capabilities_length = csc_one.length + 1;
    sink_pac_val.data[0].codec_specific_capabilities = &csc_one;
    sink_pac_val.data[0].metadata_length = 0;
    memcpy(sink_pac_val.data[1].codec_id, lc3_codec_id, sizeof(sink_pac_val.data[0].codec_id));
    sink_pac_val.data[1].codec_specific_capabilities_length = csc_two.length + 1;
    sink_pac_val.data[1].codec_specific_capabilities = &csc_two;
    sink_pac_val.data[1].metadata_length = 0;
    memcpy(sink_pac_val.data[2].codec_id, lc3_codec_id, sizeof(sink_pac_val.data[0].codec_id));
    sink_pac_val.data[2].codec_specific_capabilities_length = csc_three.length + 1;
    sink_pac_val.data[2].codec_specific_capabilities = &csc_three;
    sink_pac_val.data[2].metadata_length = 0;

    uint64_t audio_location = AUDIO_LOC_DEF_LEFT_SURROUND & AUDIO_LOC_DEF_RIGHT_SURROUND;
    memcpy(sink_audio_loc_val.sink_audio_locations, &audio_location, sizeof(sink_audio_loc_val.sink_audio_locations));

    uint16_t available_sink_contexts = CONTEXT_TYPE_MEDIA;
    uint16_t available_source_contexts = CONTEXT_TYPE_PROHIBITED;
    memcpy(avail_audio_cntxt_val.available_sink_contexts, &available_sink_contexts, sizeof(avail_audio_cntxt_val.available_sink_contexts));
    memcpy(avail_audio_cntxt_val.available_source_contexts, &available_source_contexts, sizeof(avail_audio_cntxt_val.available_source_contexts));

    uint16_t supported_sink_contexts = CONTEXT_TYPE_MEDIA;
    uint16_t supported_source_contexts = CONTEXT_TYPE_PROHIBITED;
    memcpy(supp_audio_cntxt_val.supported_sink_contexts, &supported_sink_contexts, sizeof(supp_audio_cntxt_val.supported_sink_contexts));
    memcpy(supp_audio_cntxt_val.supported_source_contexts, &supported_source_contexts, sizeof(supp_audio_cntxt_val.supported_source_contexts));



    if( !isRegister )
    {
        isRegister = 1;
        bt_conn_cb_register(&ble_pacs_conn_callbacks);
        bt_gatt_service_register(&ble_pacs_server);
    }

    return 0;
}