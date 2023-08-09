#ifndef _PASC_DATA_TYPES_H_
#define _PASC_DATA_TYPES_H_

#include <stdint.h>

#define SINK_PAC_CHRC_VAL_MAX_AMNT 10

typedef struct {
    uint8_t codec_id[5];
    uint8_t codec_specific_capabilities_length;
    void *codec_specific_capabilities;
    uint8_t metadata_length;
    void *metadata;
} sink_pac_chrc_value_data;

typedef struct {
    uint8_t number_of_pac_records;
    sink_pac_chrc_value_data data[SINK_PAC_CHRC_VAL_MAX_AMNT];
} sink_pac_chrc_value;

#define DEFINE_SINK_PAC_CHRC_VALUE(_name) \
    sink_pac_chrc_value _name;

typedef struct {
    uint8_t sink_audio_locations[4];
} sink_audio_locations_chrc_value;

#define DEFINE_SINK_AUDIO_LOCATIONS_CHRC_VALUE(_name)   \
    sink_audio_locations_chrc_value _name;

typedef struct {
    uint8_t available_sink_contexts[2];
    uint8_t available_source_contexts[2];
} available_audio_contexts_chrc_value;

#define DEFINE_AVAILABLE_AUDIO_CONTEXTS_CHRC_VALUE(_name)   \
    available_audio_contexts_chrc_value _name;

typedef struct {
    uint8_t supported_sink_contexts[2];
    uint8_t supported_source_contexts[2];
} supported_audio_contexts_chrc_value;

#define DEFINE_SUPPORTED_AUDIO_CONTEXTS_CHRC_VALUE(_name)   \
    supported_audio_contexts_chrc_value _name;

#endif