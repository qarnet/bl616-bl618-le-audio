#ifndef _AUDIO_DATA_TYPES_H_
#define _AUDIO_DATA_TYPES_H_

#include <stdint.h>

/**
 * BUFFER
*/

#define ADD_U8_TO_BUF(_buf, _val)  \
    net_buf_simple_add_u8(_buf, _val)
#define ADD_ARR_TO_BUF(_buf, _arr) \
    net_buf_simple_add_mem(_buf, _arr, sizeof(_arr))
#define ADD_AUDIO_STRUCT_TO_BUF(_buf, _struct)    \
    net_buf_simple_add_u8(_buf, _struct.length);    \
    net_buf_simple_add_u8(_buf, _struct.type);  \
    if(_struct.length >= 0x01) net_buf_simple_add_mem(_buf, _struct.value, _struct.length - 1); //length - 1, because the length includes the type

/**
 * GENERIC AUDIO
*/

enum audio_location_definitions {
    AUDIO_LOC_DEF_NOT_ALLOWED = 0x00000000,
    AUDIO_LOC_DEF_FRONT_LEFT = 0x00000001,
    AUDIO_LOC_DEF_FRONT_RIGHT = 0x00000002,
    AUDIO_LOC_DEF_FRONT_CENTER = 0x00000004,
    AUDIO_LOC_DEF_LOW_FREQ_EFFECTS_1 = 0x00000008,
    AUDIO_LOC_DEF_BACK_LEFT = 0x00000010,
    AUDIO_LOC_DEF_BACK_RIGHT = 0x00000020,
    AUDIO_LOC_DEF_FRONT_LEFT_OF_CENTER = 0x00000040,
    AUDIO_LOC_DEF_FRONT_RIGHT_OF_CENTER = 0x00000080,
    AUDIO_LOC_DEF_BACK_CENTER = 0x00000100,
    AUDIO_LOC_DEF_LOW_FREQ_EFFECTS_2 = 0x00000200,
    AUDIO_LOC_DEF_SIDE_LEFT = 0x00000400,
    AUDIO_LOC_DEF_SIDE_RIGHT = 0x00000800,
    AUDIO_LOC_DEF_TOP_FRONT_LEFT = 0x00001000,
    AUDIO_LOC_DEF_TOP_FRONT_RIGHT = 0x00002000,
    AUDIO_LOC_DEF_TOP_FRONT_CENTER = 0x00004000,
    AUDIO_LOC_DEF_TOP_CENTER = 0x00008000,
    AUDIO_LOC_DEF_TOP_BACK_LEFT = 0x00010000,
    AUDIO_LOC_DEF_TOP_BACK_RIGHT = 0x00020000,
    AUDIO_LOC_DEF_TOP_SIDE_LEFT = 0x00040000,
    AUDIO_LOC_DEF_TOP_SIDE_RIGHT = 0x00080000,
    AUDIO_LOC_DEF_TOP_BACK_CENTER = 0x00100000,
    AUDIO_LOC_DEF_BOTTOM_FRONT_CENTER = 0x00200000,
    AUDIO_LOC_DEF_BOTTOM_FRONT_LEFT = 0x00400000,
    AUDIO_LOC_DEF_BOTTOM_FRONT_RIGHT = 0x00800000,
    AUDIO_LOC_DEF_FRONT_LEFT_WIDE = 0x01000000,
    AUDIO_LOC_DEF_FRONT_RIGHT_WIDE = 0x02000000,
    AUDIO_LOC_DEF_LEFT_SURROUND = 0x04000000,
    AUDIO_LOC_DEF_RIGHT_SURROUND = 0x08000000
};

enum audio_input_type_definitions {
    AUDIO_INPUT_TYPE_DEF_UNSPECIFIED = 0x00,
    AUDIO_INPUT_TYPE_DEF_BLUETOOTH = 0x01,
    AUDIO_INPUT_TYPE_DEF_MICROPHONE = 0x02,
    AUDIO_INPUT_TYPE_DEF_ANALOG = 0x03,
    AUDIO_INPUT_TYPE_DEF_DIGITAL = 0x04,
    AUDIO_INPUT_TYPE_DEF_RADIO = 0x05,
    AUDIO_INPUT_TYPE_DEF_STREAMING = 0x06
};


enum context_type {
    CONTEXT_TYPE_PROHIBITED = 0x0000,
    CONTEXT_TYPE_UNSPECIFIED = 0x0001,
    CONTEXT_TYPE_CONVERSATIONAL = 0x0002,
    CONTEXT_TYPE_MEDIA = 0x0004,
    CONTEXT_TYPE_GAME = 0x0008,
    CONTEXT_TYPE_INSTRUCTIONAL = 0x0010,
    CONTEXT_TYPE_VOICE_ASSISTANTS = 0x0020,
    CONTEXT_TYPE_LIVE = 0x0040,
    CONTEXT_TYPE_SOUND_EFFECTS = 0x0080,
    CONTEXT_TYPE_NOTIFICATIONS = 0x0100,
    CONTEXT_TYPE_RINGTONE = 0x0200,
    CONTEXT_TYPE_ALERTS = 0x0400,
    CONTEXT_TYPE_EMERGENCY_ALARM = 0x0800
};


/**
 * CODEC SPECIFIC CAPABILITIES
*/

typedef enum  {
    CODEC_SPECIFIC_CAPABILITY_TYPE_SUPPORTED_SAMPLING_FREQUENCY = 0x01,
    CODEC_SPECIFIC_CAPABILITY_TYPE_SUPPORTED_FRAME_DURATION = 0x02,
    CODEC_SPECIFIC_CAPABILITY_TYPE_SUPPORTED_AUDIO_CHANNEL_COUNTS = 0x03,
    CODEC_SPECIFIC_CAPABILITY_TYPE_SUPPORTED_OCTETS_PER_CODEC_FRAME = 0x04,
    CODEC_SPECIFIC_CAPABILITY_TYPE_SUPPORTED_MAX_CODEC_FRAMES_PER_SDU = 0x05
} codec_specifc_capabilities_type;

typedef struct {
    uint8_t length;
    uint8_t type;
} codec_specific_capabilities_empty;

typedef struct {
    uint8_t length;
    uint8_t type;
    uint8_t value[2];
} codec_specific_capabilities_supported_sampling_frequencies;

#define CODEC_SPECIFIC_CAPABILITIES_SUPP_SAMPL_FREQ_8000Hz BIT(0)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_SAMPL_FREQ_11025Hz BIT(1)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_SAMPL_FREQ_16000Hz BIT(2)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_SAMPL_FREQ_22050Hz BIT(3)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_SAMPL_FREQ_24000Hz BIT(4)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_SAMPL_FREQ_32000Hz BIT(5)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_SAMPL_FREQ_44100Hz BIT(6)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_SAMPL_FREQ_48000Hz BIT(7)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_SAMPL_FREQ_88200Hz BIT(8)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_SAMPL_FREQ_96000Hz BIT(9)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_SAMPL_FREQ_176400Hz BIT(10)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_SAMPL_FREQ_192000Hz BIT(11)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_SAMPL_FREQ_384000Hz BIT(12)

typedef struct {
    uint8_t length;
    uint8_t type;
    uint8_t value[1];
} codec_specific_capabilities_supported_frame_durations;

#define CODEC_SPECIFIC_CAPABILITIES_SUPP_FRAME_DUR_7_5MS BIT(0)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_FRAME_DUR_10MS BIT(1)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_FRAME_DUR_7_5MS_PREF BIT(4)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_FRAME_DUR_10MS_PREF BIT(5)

typedef struct {
    uint8_t length;
    uint8_t type;
    uint8_t value[1];
} codec_specific_capabilities_supported_audio_channel_counts;

#define CODEC_SPECIFIC_CAPABILITIES_SUPP_AUDIO_CHAN_CNT_1 BIT(0)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_AUDIO_CHAN_CNT_2 BIT(1)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_AUDIO_CHAN_CNT_3 BIT(2)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_AUDIO_CHAN_CNT_4 BIT(3)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_AUDIO_CHAN_CNT_5 BIT(4)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_AUDIO_CHAN_CNT_6 BIT(5)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_AUDIO_CHAN_CNT_7 BIT(6)
#define CODEC_SPECIFIC_CAPABILITIES_SUPP_AUDIO_CHAN_CNT_8 BIT(7)

typedef struct {
    uint8_t length;
    uint8_t type;
    uint8_t value[4];
} codec_specific_capabilities_supported_octets_per_codec_frame;

typedef struct {
    uint8_t length;
    uint8_t type;
    uint8_t value[1];
} codec_specific_capabilities_supported_max_codec_frames_per_sdu;


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
    uint8_t value[1];
} codec_specific_configuration_sampling_frequency;

#define CODEC_SPECIFIC_CONFIGURATION_SAMP_FREQ_8000Hz 0x01
#define CODEC_SPECIFIC_CONFIGURATION_SAMP_FREQ_11025Hz 0x01
#define CODEC_SPECIFIC_CONFIGURATION_SAMP_FREQ_16000Hz 0x03
#define CODEC_SPECIFIC_CONFIGURATION_SAMP_FREQ_22050Hz 0x04
#define CODEC_SPECIFIC_CONFIGURATION_SAMP_FREQ_24000Hz 0x05
#define CODEC_SPECIFIC_CONFIGURATION_SAMP_FREQ_32000Hz 0x06
#define CODEC_SPECIFIC_CONFIGURATION_SAMP_FREQ_44100Hz 0x07
#define CODEC_SPECIFIC_CONFIGURATION_SAMP_FREQ_48000Hz 0x08
#define CODEC_SPECIFIC_CONFIGURATION_SAMP_FREQ_88200Hz 0x09
#define CODEC_SPECIFIC_CONFIGURATION_SAMP_FREQ_96000Hz 0x0A
#define CODEC_SPECIFIC_CONFIGURATION_SAMP_FREQ_176400Hz 0x0B
#define CODEC_SPECIFIC_CONFIGURATION_SAMP_FREQ_192000Hz 0x0C
#define CODEC_SPECIFIC_CONFIGURATION_SAMP_FREQ_384000Hz 0x0D

#define DEFINE_CSC_SAMPLING_FREQUENCY(_name, _data)    \
    codec_specific_configuration_sampling_frequency _name = {   \
        .length = 0x02,     \
        .type = 0x01};  \
    (_name.length - 1) == sizeof(_data) ? _name.value = _data : ;

#define CODEC_SPECIFIC_CONFIGURATION_FRAME_DUR_7_5MS 0x00
#define CODEC_SPECIFIC_CONFIGURATION_FRAME_DUR_10MS 0x01


typedef struct {
    uint8_t length;
    uint8_t type;
    uint8_t value[1];
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
    uint8_t value[1];
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
    uint8_t value[1];
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
    uint8_t value[1];
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