#ifndef _AUDIO_DATA_TYPES_H_
#define _AUDIO_DATA_TYPES_H_

#include <stdint.h>

/**
 * CODEC SPECIFIC CONFIGURATION
*/

typedef enum  {
    CSC_TYPE_SAMPLING_FREQUENCY = 0x01,
    CSC_TYPE_FRAME_DURATION = 0x02,
    CSC_TYPE_AUDIO_CHANNEL_ALLOCATION = 0x03,
    CSC_TYPE_OCTETS_PER_CODEC_FRAME = 0x04,
    CSC_TYPE_CODEC_FRAME_BLOCKS_PER_SDU = 0x05
} codec_specifc_configuration_type;

typedef struct {
    uint8_t length;
    uint8_t type;
} codec_specific_configuration_empty;

typedef struct {
    uint8_t length;
    uint8_t type;
    uint8_t value;
} codec_specific_configuration_sampling_frequency;

#define DEFINE_CSC_SAMPLING_FREQUENCY(_name, _data)    \
    codec_specific_configuration_sampling_frequency _name = {   \
        .length = 0x02,     \
        .type = 0x01};  \
    (_name.length - 1) == sizeof(_data) ? _name.value = _data : ;


typedef struct {
    uint8_t length;
    uint8_t type;
    uint8_t value;
} codec_specific_configuration_frame_duration;

#define DEFINE_CSC_CONFIGURATION_FRAME_DURATION(_name, _data)    \
    codec_specific_configuration_sampling_frequency _name = {   \
        .length = 0x02,     \
        .type = 0x02};  \
    (_name.length - 1) == sizeof(_data) ? _name.value = _data : ;


typedef struct {
    uint8_t length;
    uint8_t type;
    uint8_t value[4];
} codec_specific_configuration_audio_channel_allocation;

#define DEFINE_CSC_AUDIO_CHANNEL_ALLOCATION(_name, _data)    \
    codec_specific_configuration_sampling_frequency _name = {   \
        .length = 0x05,     \
        .type = 0x03};  \
    (_name.length - 1) == sizeof(_data) ? memcpy(_name.value, _data, sizeof(_data)) : ;


typedef struct {
    uint8_t length;
    uint8_t type;
    uint8_t value[2];
} codec_specific_configuration_octets_per_codec_frame;

#define DEFINE_CSC_OCTETS_PER_CODEC_FRAME(_name, _data)    \
    codec_specific_configuration_sampling_frequency _name = {   \
        .length = 0x03,     \
        .type = 0x04};  \
    (_name.length - 1) == sizeof(_data) ? memcpy(_name.value, _data, sizeof(_data)) : ;


typedef struct {
    uint8_t length;
    uint8_t type;
    uint8_t value;
} codec_specific_configuration_codec_frame_blocks_per_sdu;

#define DEFINE_CSC_CODEC_FRAME_BLOCKS_PER_SDU(_name, _data)    \
    codec_specific_configuration_sampling_frequency _name = {   \
        .length = 0x02,     \
        .type = 0x05};  \
    (_name.length - 1) == sizeof(_data) ? _name.value = _data : ;


/**
 * METADATA
*/

#define METADATA_BUFFER_MAX_SIZE 256
static uint8_t metadata_buffer[METADATA_BUFFER_MAX_SIZE];

typedef enum {
    META_TYPE_PREFERRED_AUDIO_CONTEXTS = 0x01,
    META_TYPE_STREAMING_AUDIO_CONTEXTS = 0x02,
    META_TYPE_PROGRAM_INFO = 0x03,
    META_TYPE_LANGUAGE = 0x04,
    META_TYPE_CCID_LIST = 0x05,
    META_TYPE_PARENTAL_RATING = 0x06,
    META_TYPE_PROGRAM_INFO_URI = 0x07,
    META_TYPE_EXTENDED_METADATA = 0xfe,
    META_TYPE_VENDOR_SPECIFIC = 0xff,
    META_TYPE_AUDIO_ACTIVE_STATE = 0x08,
    META_TYPE_BROADCAST_ADUIO_IMMEDIATE_RENDERING_FLAG = 0x09,
} metadata_type;

typedef struct {
    uint8_t length;
    uint8_t type;
} metadata_empty;

typedef struct {
    uint8_t length;
    uint8_t type;
    uint8_t value[2];
} metadata_preferred_audio_contexts;

#define DEFINE_META_PREFERRED_AUDIO_CONTEXT(_name, _data)    \
    codec_specific_configuration_sampling_frequency _name = {   \
        .length = 0x03,     \
        .type = 0x01};  \
    (_name.length - 1) == sizeof(_data) ? memcpy(_name.value, _data, sizeof(_data)) : ;


typedef struct {
    uint8_t length;
    uint8_t type;
    uint8_t value[2];
} metadata_streaming_audio_contexts;

#define DEFINE_META_AUDIO_CONTEXTS(_name, _data)    \
    codec_specific_configuration_sampling_frequency _name = {   \
        .length = 0x03,     \
        .type = 0x02};  \
    (_name.length - 1) == sizeof(_data) ? memcpy(_name.value, _data, sizeof(_data)) : ;


typedef struct {
    uint8_t length;
    uint8_t type;
    void *value;
} metadata_program_info;

#define DEFINE_META_PROGRAM_INFO(_name, _data, len)    \
    codec_specific_configuration_sampling_frequency _name = {   \
        .type = 0x03};  \
    _name.length = _len;    \
    _name.value = metadata_buffer;  \
    memcpy(metadata_buffer, _data, _len);


typedef struct {
    uint8_t length;
    uint8_t type;
    uint8_t value[3];
} metadata_language;

#define DEFINE_META_LANGUAGE(_name, _data)    \
    codec_specific_configuration_sampling_frequency _name = {   \
        .length = 0x04,     \
        .type = 0x04};  \
    (_name.length - 1) == sizeof(_data) ? memcpy(_name.value, _data, sizeof(_data)) : ;


typedef struct {
    uint8_t length;
    uint8_t type;
    void *value;
} metadata_ccid_list;

#define DEFINE_META_CCID_LIST(_name, _data, len)    \
    codec_specific_configuration_sampling_frequency _name = {   \
        .type = 0x05};  \
    _name.length = _len;    \
    _name.value = metadata_buffer;  \
    memcpy(metadata_buffer, _data, _len);


typedef struct {
    uint8_t length;
    uint8_t type;
    uint8_t value;
} metadata_parental_rating;

#define DEFINE_META_PARENTAL_RATING(_name, _data)    \
    codec_specific_configuration_sampling_frequency _name = {   \
        .length = 0x02,     \
        .type = 0x06};  \
    (_name.length - 1) == sizeof(_data) ? _name.value = _data : ;


typedef struct {
    uint8_t length;
    uint8_t type;
    void *value;
} metadata_program_info_uri;

#define DEFINE_META_PROGRAM_INFO_URI(_name, _data, len)    \
    codec_specific_configuration_sampling_frequency _name = {   \
        .type = 0x07};  \
    _name.length = _len;    \
    _name.value = metadata_buffer;  \
    memcpy(metadata_buffer, _data, _len);



typedef struct {
    uint8_t length;
    uint8_t type;
    void *value;
} metadata_extended_metadata;

#define DEFINE_META_EXTENDED_METADATA(_name, _data, len)    \
    codec_specific_configuration_sampling_frequency _name = {   \
        .type = 0xfe};  \
    _name.length = _len;    \
    _name.value = metadata_buffer;  \
    memcpy(metadata_buffer, _data, _len);


typedef struct {
    uint8_t length;
    uint8_t type;
    void *value;
} metadata_vendor_specific;

#define DEFINE_META_VENDOR_SPECIFIC(_name, _data, len)    \
    codec_specific_configuration_sampling_frequency _name = {   \
        .type = 0xff};  \
    _name.length = _len;    \
    _name.value = metadata_buffer;  \
    memcpy(metadata_buffer, _data, _len);


typedef struct {
    uint8_t length;
    uint8_t type;
    uint8_t value;
} metadata_audio_active_state;

#define DEFINE_META_AUDIO_ACTIVE_STATE(_name, _data)    \
    codec_specific_configuration_sampling_frequency _name = {   \
        .length = 0x02,     \
        .type = 0x08};  \
    (_name.length - 1) == sizeof(_data) ? _name.value = _data : ;


typedef struct {
    uint8_t length;
    uint8_t type;
} metadata_broadcast_audio_immediate_rendering_flag;

#define DEFINE_META_BROADCAST_AUDIO_IMMEDIATE_RENERING_FLAG(_name)    \
    codec_specific_configuration_sampling_frequency _name = {   \
        .length = 0x01,     \
        .type = 0x09};


#endif