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
static void ble_ase_control_point_notify_val();
static struct bt_gatt_attr *get_attr(u8_t index);

static struct bt_conn *ble_ascs_conn;
static struct bt_gatt_exchange_params exchg_mtu;

static int tx_mtu_size = 20;
static u8_t isRegister = 0;

ascs_sm_state current_state = ASCS_SM_STATE_IDLE;
ascs_sm_evt new_event;
TaskHandle_t state_machine_task_handle;

DEFINE_SINK_ASE_VALUE(sink_ase_val);
DEFINE_ASE_CONTROL_POINT_VALUE(ase_cp_val);

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

NET_BUF_SIMPLE_DEFINE(buffer, 256);
NET_BUF_SIMPLE_DEFINE(ase_cp_buf, 256);

static int ble_sink_ase_recv_rd(struct bt_conn *_conn,	const struct bt_gatt_attr *_attr,
                                        void *_buf, u16_t _len, u16_t _offset)
{
    net_buf_simple_init(&buffer, 0);

    if((current_state == ASCS_SM_STATE_IDLE) || (current_state == ASCS_SM_STATE_RELEASING))
    {
        ADD_U8_TO_BUF(&buffer, sink_ase_val.ase_id);
        ADD_U8_TO_BUF(&buffer, sink_ase_val.ase_state);

        goto end;
    }
    else if(current_state == ASCS_SM_STATE_CODEC_CONFIGURED)
    {
        ADD_U8_TO_BUF(&buffer, sink_ase_val.ase_id);
        ADD_U8_TO_BUF(&buffer, sink_ase_val.ase_state);

        sink_ase_csc_codec_configured additional_data;
        memcpy(&additional_data, sink_ase_val.additional_ase_params, (sizeof(additional_data)));

        ADD_U8_TO_BUF(&buffer, additional_data.framing);
        ADD_U8_TO_BUF(&buffer, additional_data.preferred_phy);
        ADD_U8_TO_BUF(&buffer, additional_data.preferred_retransmission_number);
        ADD_ARR_TO_BUF(&buffer, additional_data.max_transport_lateny);
        ADD_ARR_TO_BUF(&buffer, additional_data.presentation_delay_min);
        ADD_ARR_TO_BUF(&buffer, additional_data.presentation_delay_max);
        ADD_ARR_TO_BUF(&buffer, additional_data.preferred_presentation_delay_min);
        ADD_ARR_TO_BUF(&buffer, additional_data.preferred_presentation_delay_max);
        ADD_ARR_TO_BUF(&buffer, additional_data.codec_id);
        ADD_U8_TO_BUF(&buffer, additional_data.codec_specific_configuration_length);

        if(additional_data.codec_specific_configuration_length == 0x00)
        {
            goto end;
        }

        codec_specific_configuration_empty csc_type_finder;
        memcpy(&csc_type_finder, additional_data.codec_specific_configuration, sizeof(csc_type_finder));

        switch(csc_type_finder.type)
        {
            case CSC_TYPE_SAMPLING_FREQUENCY: ;
                codec_specific_configuration_sampling_frequency csc_sf;
                memcpy(&csc_sf, additional_data.codec_specific_configuration, sizeof(csc_sf));
                ADD_VARIED_U8_TO_BUF(&buffer, csc_sf);
                break;
            case CSC_TYPE_FRAME_DURATION: ;
                codec_specific_configuration_frame_duration csc_fd;
                memcpy(&csc_fd, additional_data.codec_specific_configuration, sizeof(csc_fd));
                ADD_VARIED_U8_TO_BUF(&buffer, csc_sf);
                break;
            case CSC_TYPE_AUDIO_CHANNEL_ALLOCATION: ;
                codec_specific_configuration_audio_channel_allocation csc_aca;
                memcpy(&csc_aca, additional_data.codec_specific_configuration, sizeof(csc_aca));
                ADD_VARIED_U8_TO_BUF(&buffer, csc_sf);
                break;
            case CSC_TYPE_OCTETS_PER_CODEC_FRAME: ;
                codec_specific_configuration_octets_per_codec_frame csc_opcf;
                memcpy(&csc_opcf, additional_data.codec_specific_configuration, sizeof(csc_opcf));
                ADD_VARIED_U8_TO_BUF(&buffer, csc_sf);
                break;
            case CSC_TYPE_CODEC_FRAME_BLOCKS_PER_SDU: ;
                codec_specific_configuration_codec_frame_blocks_per_sdu csc_cfbps;
                memcpy(&csc_cfbps, additional_data.codec_specific_configuration, sizeof(csc_cfbps));
                ADD_VARIED_U8_TO_BUF(&buffer, csc_sf);
                break;
        }

        goto end;
    }
    else if(current_state == ASCS_SM_STATE_QOS_CONFIGURED)
    {
        ADD_U8_TO_BUF(&buffer, sink_ase_val.ase_id);
        ADD_U8_TO_BUF(&buffer, sink_ase_val.ase_state);

        sink_ase_csc_qos_configured additional_data;
        memcpy(&additional_data, sink_ase_val.additional_ase_params, (sizeof(additional_data)));

        ADD_U8_TO_BUF(&buffer, additional_data.cig_id);
        ADD_U8_TO_BUF(&buffer, additional_data.cis_id);
        ADD_ARR_TO_BUF(&buffer, additional_data.sdu_interval);
        ADD_U8_TO_BUF(&buffer, additional_data.framing);
        ADD_U8_TO_BUF(&buffer, additional_data.phy);
        ADD_ARR_TO_BUF(&buffer, additional_data.max_sdu);
        ADD_U8_TO_BUF(&buffer, additional_data.retransmission_number);
        ADD_ARR_TO_BUF(&buffer, additional_data.max_transport_latency);
        ADD_ARR_TO_BUF(&buffer, additional_data.presentation_delay);

        goto end;
    }
    else if((current_state == ASCS_SM_STATE_ENABLING) ||
            (current_state == ASCS_SM_STATE_STREAMING) ||
            (current_state == ASCS_SM_STATE_DISABLING))
    {
        ADD_U8_TO_BUF(&buffer, sink_ase_val.ase_id);
        ADD_U8_TO_BUF(&buffer, sink_ase_val.ase_state);

        sink_ase_csc_enabling_streaming_disabling additional_data;
        memcpy(&additional_data, sink_ase_val.additional_ase_params, (sizeof(additional_data)));

        ADD_U8_TO_BUF(&buffer, additional_data.cig_id);
        ADD_U8_TO_BUF(&buffer, additional_data.cis_id);
        ADD_U8_TO_BUF(&buffer, additional_data.metadata_length);

        metadata_empty metadata_type_finder;
        memcpy(&metadata_type_finder, additional_data.metadata, sizeof(metadata_type_finder));

        switch(metadata_type_finder.type)
        {
            case META_TYPE_PREFERRED_AUDIO_CONTEXTS: ;
                metadata_preferred_audio_contexts meta_pac;
                memcpy(&meta_pac, additional_data.metadata, sizeof(meta_pac));
                ADD_VARIED_ARR_TO_BUF(&buffer, meta_pac);
                break;
            case META_TYPE_STREAMING_AUDIO_CONTEXTS: ;
                metadata_streaming_audio_contexts meta_sac;
                memcpy(&meta_sac, additional_data.metadata, sizeof(meta_sac));
                ADD_VARIED_ARR_TO_BUF(&buffer, meta_sac);
                break;
            case META_TYPE_PROGRAM_INFO: ;
                metadata_program_info meta_pi;
                memcpy(&meta_pi, additional_data.metadata, sizeof(meta_pi));
                ADD_VARIED_POINTER_TO_BUF(&buffer, meta_pi);
                break;
            case META_TYPE_LANGUAGE: ;
                metadata_language meta_l;
                memcpy(&meta_l, additional_data.metadata, sizeof(meta_l));
                ADD_VARIED_ARR_TO_BUF(&buffer, meta_l);
                break;
            case META_TYPE_CCID_LIST: ;
                metadata_ccid_list meta_cl;
                memcpy(&meta_cl, additional_data.metadata, sizeof(meta_cl));
                ADD_VARIED_POINTER_TO_BUF(&buffer, meta_cl);
                break;
            case META_TYPE_PARENTAL_RATING: ;
                metadata_parental_rating meta_pr;
                memcpy(&meta_pr, additional_data.metadata, sizeof(meta_pr));
                ADD_VARIED_U8_TO_BUF(&buffer, meta_pr);
                break;
            case META_TYPE_PROGRAM_INFO_URI: ;
                metadata_program_info_uri meta_pii;
                memcpy(&meta_pii, additional_data.metadata, sizeof(meta_pii));
                ADD_VARIED_POINTER_TO_BUF(&buffer, meta_pii);
                break;
            case META_TYPE_EXTENDED_METADATA: ;
                metadata_extended_metadata meta_em;
                memcpy(&meta_em, additional_data.metadata, sizeof(meta_em));
                ADD_VARIED_POINTER_TO_BUF(&buffer, meta_em);
                break;
            case META_TYPE_VENDOR_SPECIFIC: ;
                metadata_vendor_specific meta_vs;
                memcpy(&meta_vs, additional_data.metadata, sizeof(meta_vs));
                ADD_VARIED_POINTER_TO_BUF(&buffer, meta_vs);
                break;
            case META_TYPE_AUDIO_ACTIVE_STATE: ;
                metadata_audio_active_state meta_aas;
                memcpy(&meta_aas, additional_data.metadata, sizeof(meta_aas));
                ADD_VARIED_U8_TO_BUF(&buffer, meta_aas);
                break;
            case META_TYPE_BROADCAST_ADUIO_IMMEDIATE_RENDERING_FLAG: ;
                metadata_broadcast_audio_immediate_rendering_flag meta_bairf;
                memcpy(&meta_bairf, additional_data.metadata, sizeof(meta_bairf));
                ADD_U8_TO_BUF(&buffer, meta_bairf.length);
                ADD_U8_TO_BUF(&buffer, meta_bairf.type);
                break;

            goto end;
        }
    }

    end:

    memcpy(_buf, buffer.data, buffer.len);
    net_buf_simple_reset(&buffer);
    
    return buffer.len;
}

static int ble_ase_control_point_recv_wr(struct bt_conn *_conn, const struct bt_gatt_attr *_attr,
                                        const void *_buf, u16_t _len, u16_t _offset, u8_t _flags)
{
    BT_WARN("recv data len=%d, offset=%d, flag=%d", _len, _offset, _flags);

    uint8_t buf_u8[_len];
    uint8_t opcode;

    memcpy(buf_u8, _buf, _len);
    opcode = buf_u8[0];

    switch(opcode)
    {
        case ASE_CP_CONFIG_CODEC:
            if(current_state == ASCS_SM_STATE_IDLE ||
               current_state == ASCS_SM_STATE_CODEC_CONFIGURED ||
               current_state == ASCS_SM_STATE_QOS_CONFIGURED)
            {
                pdPASS == xTaskNotifyFromISR(state_machine_task_handle, ASCS_SM_EVT_CONFIG_CODEC, eSetValueWithOverwrite, 0);
            }
            break;
        case ASE_CP_CONFIG_QOS:
            if(current_state == ASCS_SM_STATE_CODEC_CONFIGURED ||
               current_state == ASCS_SM_STATE_QOS_CONFIGURED)
            {
                pdPASS == xTaskNotifyFromISR(state_machine_task_handle, ASCS_SM_EVT_CONFIG_QOS, eSetValueWithOverwrite, 0);
            }
            break;
        case ASE_CP_ENABLE:
            if(current_state == ASCS_SM_STATE_QOS_CONFIGURED)
            {
                pdPASS == xTaskNotifyFromISR(state_machine_task_handle, ASCS_SM_EVT_ENABLE, eSetValueWithOverwrite, 0);
            }
            break;
        case ASE_CP_RECEIVER_START_READY:
            if(current_state == ASCS_SM_STATE_ENABLING)
            {
                pdPASS == xTaskNotifyFromISR(state_machine_task_handle, ASCS_SM_EVT_RECEIVER_START_READY, eSetValueWithOverwrite, 0);
            }
            break;
        case ASE_CP_DISABLE:
            if(current_state == ASCS_SM_STATE_ENABLING ||
               current_state == ASCS_SM_STATE_STREAMING)
            {
                pdPASS == xTaskNotifyFromISR(state_machine_task_handle, ASCS_SM_EVT_DISABLE, eSetValueWithOverwrite, 0);
            }
            break;
        case ASE_CP_RECEIVER_STOP_READY:
            if(current_state == ASCS_SM_STATE_DISABLING)
            {
                pdPASS == xTaskNotifyFromISR(state_machine_task_handle, ASCS_SM_EVT_DISABLE, eSetValueWithOverwrite, 0);
            }
            break;
        case ASE_CP_UPDATE_METADATA:
            if(current_state == ASCS_SM_STATE_ENABLING ||
               current_state == ASCS_SM_STATE_STREAMING)
            {
                pdPASS == xTaskNotifyFromISR(state_machine_task_handle, ASCS_SM_EVT_UPDATE_METADATA, eSetValueWithOverwrite, 0);
            }
            break;
        case ASE_CP_RELEASE:
            if(current_state == ASCS_SM_STATE_CODEC_CONFIGURED ||
               current_state == ASCS_SM_STATE_QOS_CONFIGURED ||
               current_state == ASCS_SM_STATE_ENABLING ||
               current_state == ASCS_SM_STATE_STREAMING ||
               current_state == ASCS_SM_STATE_DISABLING)
            {
                pdPASS == xTaskNotifyFromISR(state_machine_task_handle, ASCS_SM_EVT_RELEASE, eSetValueWithOverwrite, 0);
            }
            break;
        default:
            printf("Undefined Opcopde\r\n");
            break;
    }

    return _len;
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

static void ble_ase_control_point_notify_val()
{
    int err;

    net_buf_simple_init(&ase_cp_buf, 0);

    ADD_U8_TO_BUF(&ase_cp_buf, ase_cp_val.opcode);
    ADD_U8_TO_BUF(&ase_cp_buf, ase_cp_val.number_of_ases);

    if(0xFF != ase_cp_val.number_of_ases)
    {
        net_buf_simple_add_mem(&ase_cp_buf, ase_cp_val.ase_data, ase_cp_val.number_of_ases * sizeof(ase_cp_data_array));
    }
    else
    {
        net_buf_simple_add_mem(&ase_cp_buf, ase_cp_val.ase_data, 1 * sizeof(ase_cp_data_array));
    }

    err = bt_gatt_notify(ble_ascs_conn, get_attr(BT_CHAR_ASE_CONTROL_POINT_NOTIFY_ATTR_VAL_INDEX), ase_cp_buf.data, ase_cp_buf.len);
    if(err != 0)
    {
        printf("Error notifying of ase control point value (err %d)\r\n", err);
    }

    net_buf_simple_reset(&ase_cp_buf);
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

void stateMachineTask(void *_params)
{
    uint32_t event;
    for(;;)
    {
        printf("TASK\r\n");
        if(pdTRUE == xTaskNotifyWaitIndexed(1, 0, 0xffffffff, &event, pdMS_TO_TICKS(0xFFFFFF)))
        {
            current_state = stateMachine[current_state][event](NULL);
        }
        else
        {
            // Error
        }

        k_sleep(1000);
    }

    vTaskDelete(NULL);
}

void ascs_init()
{
    int err;
    printf("ASCS Init\r\n");

    if( !isRegister )
    {
        isRegister = 1;
        bt_conn_cb_register(&ble_ascs_conn_callbacks);
        bt_gatt_service_register(&ble_ascs_server);
        err = xTaskCreate(stateMachineTask, "taskNameStateMachine", 1000, NULL, 0, &state_machine_task_handle);
        if(pdPASS != err)
        {
            printf("Error creating ascs task\r\n");
            vTaskDelete(state_machine_task_handle);
        }
    }

    // pdPASS == xTaskNotifyIndexed(state_machine_task_handle, 1, ASCS_SM_EVT_ENABLE, eSetValueWithOverwrite);
    // pdPASS == xTaskNotifyFromISR(state_machine_task_handle, ASCS_SM_EVT_ENABLE, eSetValueWithOverwrite, 0);

    return;
}



