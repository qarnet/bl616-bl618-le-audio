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
#include "audioDataTypes.h"
#include "ascsDataTypes.h"

#define TP_PRIO configMAX_PRIORITIES - 5

static void ble_ascs_connected(struct bt_conn *conn, u8_t err);
static void ble_ascs_disconnected(struct bt_conn *conn, u8_t reason);
static struct bt_gatt_attr *get_attr(u8_t index);

static struct bt_conn *ble_ascs_conn;
static struct bt_gatt_exchange_params exchg_mtu;

static int tx_mtu_size = 20;
static u8_t isRegister = 0;

ascs_sm_state current_state = ASCS_SM_STATE_IDLE;
ascs_sm_evt new_event;

DEFINE_SINK_ASE_VALUE(sink_ase_val);

static struct bt_conn_cb ble_ascs_conn_callbacks = {
	.connected	=   ble_ascs_connected,
	.disconnected	=   ble_ascs_disconnected,
};

static void ble_ascs_tx_mtu_size(struct bt_conn *conn, u8_t err,
			  struct bt_gatt_exchange_params *params)
{
   if(!err)
   {
        tx_mtu_size = bt_gatt_get_mtu(ble_ascs_conn);
        BT_WARN("ble tp echange mtu size success, mtu size: %d", tx_mtu_size);
   }
   else
   {
        BT_WARN("ble tp echange mtu size failure, err: %d", err);
   }
}

static void ble_ascs_connected(struct bt_conn *conn, u8_t err)
{
    if(err || conn->type != BT_CONN_TYPE_LE)
    {
        return;
    }

    int tx_octets = 0x00fb;
    int tx_time = 0x0848;
    int ret = -1;

    BT_INFO("%s",__func__);
    ble_ascs_conn = conn;

    //set data length after connected.
    ret = bt_le_set_data_len(ble_ascs_conn, tx_octets, tx_time);
    if(!ret)
    {
        BT_WARN("ble tp set data length success.");
    }
    else
    {
        BT_WARN("ble tp set data length failure, err: %d\n", ret);
    }

    //exchange mtu size after connected.
    exchg_mtu.func = ble_ascs_tx_mtu_size;
    ret = bt_gatt_exchange_mtu(ble_ascs_conn, &exchg_mtu);
    if (!ret) {
        BT_WARN("ble tp exchange mtu size pending.");
    } else {
        BT_WARN("ble tp exchange mtu size failure, err: %d", ret);
    }
}

static void ble_ascs_disconnected(struct bt_conn *conn, u8_t reason)
{ 
    if(conn->type != BT_CONN_TYPE_LE)
    {
        return;
    }

    BT_INFO("%s",__func__);

    ble_ascs_conn = NULL;
}

static int ble_sink_ase_recv_rd(struct bt_conn *_conn,	const struct bt_gatt_attr *_attr,
                                        void *_buf, u16_t _len, u16_t _offset)
{
    
    if((current_state == ASCS_SM_STATE_IDLE) || (current_state == ASCS_SM_STATE_RELEASING))
    {
        uint8_t buf[2];
        buf[0] = sink_ase_val.ase_id;
        buf[1] = sink_ase_val.ase_state;
        _len = sizeof(buf);
        memcpy(_buf, buf, _len);
    }
    else if(current_state == ASCS_SM_STATE_CODEC_CONFIGURED)
    {
        uint8_t buf[256];
        buf[0] = sink_ase_val.ase_id;
        buf[1] = sink_ase_val.ase_state;

        codec_specific_configuration_empty csc_type_finder;
        sink_ase_csc_codec_configured additional_data = {
            .codec_specific_configuration = &csc_type_finder,
        };
        memcpy(&additional_data, &sink_ase_val.additional_ase_params, sizeof(additional_data) - 1);
        memcpy(&buf[2], &sink_ase_val.additional_ase_params, sizeof(additional_data) - 1);

        if(additional_data.codec_specific_configuration_length == 0x00)
        {
            goto end;
        }

        memcpy(&additional_data, &sink_ase_val.additional_ase_params, (sizeof(additional_data) - 1) + sizeof(csc_type_finder));
        switch(csc_type_finder.type)
        {
            case CSC_TYPE_SAMPLING_FREQUENCY: ;
                codec_specific_configuration_sampling_frequency csc_a;
                additional_data.codec_specific_configuration = &csc_a;
                memcpy(&additional_data, &sink_ase_val.additional_ase_params, (sizeof(additional_data) - 1) + sizeof(csc_a));
                break;
            case CSC_TYPE_FRAME_DURATION: ;
                codec_specific_configuration_frame_duration csc_b;
                additional_data.codec_specific_configuration = &csc_b;
                memcpy(&additional_data, &sink_ase_val.additional_ase_params, (sizeof(additional_data) - 1) + sizeof(csc_b));
                break;
            case CSC_TYPE_AUDIO_CHANNEL_ALLOCATION: ;
                codec_specific_configuration_audio_channel_allocation csc_c;
                additional_data.codec_specific_configuration = &csc_c;
                memcpy(&additional_data, &sink_ase_val.additional_ase_params, (sizeof(additional_data) - 1) + sizeof(csc_c));
                break;
            case CSC_TYPE_OCTETS_PER_CODEC_FRAME: ;
                codec_specific_configuration_octets_per_codec_frame csc_d;
                additional_data.codec_specific_configuration = &csc_d;
                memcpy(&additional_data, &sink_ase_val.additional_ase_params, (sizeof(additional_data) - 1) + sizeof(csc_d));
                break;
            case CSC_TYPE_CODEC_FRAME_BLOCKS_PER_SDU: ;
                codec_specific_configuration_codec_frame_blocks_per_sdu csc_e;
                additional_data.codec_specific_configuration = &csc_e;
                memcpy(&additional_data, &sink_ase_val.additional_ase_params, (sizeof(additional_data) - 1) + sizeof(csc_e));
                break;
        }
        memcpy(&buf[sizeof(additional_data) - 1], additional_data.codec_specific_configuration, additional_data.codec_specific_configuration_length);
    }
    else if(current_state == ASCS_SM_STATE_QOS_CONFIGURED)
    {

    }
    else if((current_state == ASCS_SM_STATE_ENABLING) ||
            (current_state == ASCS_SM_STATE_STREAMING) ||
            (current_state == ASCS_SM_STATE_DISABLING))
    {

    }

    end:
    
    return _len;
}

static int ble_ase_control_point_recv_wr(struct bt_conn *conn, const struct bt_gatt_attr *attr,
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

static void ble_sink_ase_notify_ccc_changed(const struct bt_gatt_attr *attr, u16_t value)
{
    int err = -1;
    char data[244] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

    while(1)
    {
        err = bt_gatt_notify(ble_ascs_conn, get_attr(BT_CHAR_SINK_ASE_NOTIFY_ATTR_VAL_INDEX), data, (tx_mtu_size - 3));
        BT_WARN("ble tp send notify : %d", err);
    }
}

static void ble_ase_control_point_notify_ccc_changed(const struct bt_gatt_attr *attr, u16_t value)
{
    int err = -1;
    char data[244] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};

    while(1)
    {
        err = bt_gatt_notify(ble_ascs_conn, get_attr(BT_CHAR_ASE_CONTROL_POINT_NOTIFY_ATTR_VAL_INDEX), data, (tx_mtu_size - 3));
        BT_WARN("ble tp send notify : %d", err);
    }
}

static struct bt_gatt_attr attrs[]= {
	BT_GATT_PRIMARY_SERVICE(BT_UUID_SVC_BLE_ASCS),

    BT_GATT_CHARACTERISTIC(BT_UUID_CHRC_SINK_ASE,
							BT_GATT_CHRC_READ | BT_GATT_CHRC_NOTIFY,
							BT_GATT_PERM_READ_ENCRYPT,
							ble_sink_ase_recv_rd,
							NULL,
							NULL),
    BT_GATT_CCC(ble_sink_ase_notify_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),

	BT_GATT_CHARACTERISTIC(BT_UUID_CHRC_ASE_CONTROL_POINT,
							BT_GATT_CHRC_WRITE |BT_GATT_CHRC_WRITE_WITHOUT_RESP | BT_GATT_CHRC_NOTIFY,
							BT_GATT_PERM_WRITE_ENCRYPT,
							NULL,
							ble_ase_control_point_recv_wr,
							NULL),
	BT_GATT_CCC(ble_ase_control_point_notify_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
};

struct bt_gatt_attr *get_attr(u8_t index)
{
	return &attrs[index];
}

struct bt_gatt_service ble_ascs_server = BT_GATT_SERVICE(attrs);

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

void ascs_init()
{
    printf("ASCS Init\n");

    
    return;

    if( !isRegister )
    {
        isRegister = 1;
        bt_conn_cb_register(&ble_ascs_conn_callbacks);
        bt_gatt_service_register(&ble_ascs_server);
    }
}


