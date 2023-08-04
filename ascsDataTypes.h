#ifndef _ASCS_DATA_TYPES_H_
#define _ASCS_DATA_TYPES_H_

#include <stdint.h>

#define ADDITIONAL_ASE_PARAMS_BUFFER_MAX_SIZE 256
static uint8_t additional_ase_params_buffer[ADDITIONAL_ASE_PARAMS_BUFFER_MAX_SIZE];

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

typedef struct {
    uint8_t ase_id;
    uint8_t ase_state;
    void *additional_ase_params;
} sink_ase_value;

#define DEFINE_SINK_ASE_VALUE(_name)    \
    sink_ase_value _name = {    \
        .additional_ase_params = additional_ase_params_buffer};

typedef struct {
    uint8_t preferred_phy;
    uint8_t preferred_retransmission_number;
    uint8_t max_transport_lateny[2];
    uint8_t presentation_delay_min[3];
    uint8_t presentation_delay_max[3];
    uint8_t preferred_presentation_delay_min[3];
    uint8_t preferred_presentation_delay_max[3];
    uint8_t codec_id[5];
    uint8_t codec_specific_configuration_length;
    void *codec_specific_configuration;
} sink_ase_csc_codec_configured;

#define DEFINE_SINK_ASE_CODEC_CONFIGURED(_name, _codec_data, _codec_data_len)     \
    sink_ase_csc_codec_configured _name = {   \
        .codec_specific_configuration_length = _codec_data_len, \
        .codec_specific_configuration = _codec_data};


typedef struct {
    uint8_t cig_id;
    uint8_t cis_id;
    uint8_t sdu_interval[3];
    uint8_t framing;
    uint8_t phy;
    uint8_t max_sdu[2];
    uint8_t retransmission_number;
    uint8_t max_transport_latency[2];
    uint8_t presentation_delay[3];
} sink_ase_csc_qos_configured;

#define DEFINE_SINK_ASE_QOS_CONFIGURED(_name)     \
    sink_ase_csc_qos_configured _name;


typedef struct {
    uint8_t metadata_length;
    uint8_t *metadata;
} sink_ase_csc_enabling_streaming_disabling; 

#define DEFINE_SINK_ASE_ENABLING_STREAMING_DISABLING(_name, _metadata, _,metadata_len)     \
    sink_ase_csc_enabling_streaming_disabling _name = {   \
        .metadata_length = _metadata_len,   \
        .metadata = _metadata};

#endif