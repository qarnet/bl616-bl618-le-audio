#include "audio.h"
#include "audioDataTypes.h"


int extractMetadata(void *_raw_metadata, struct net_buf_simple *_buf)
{
    metadata_empty metadata_type_finder;
    memcpy(&metadata_type_finder, _raw_metadata, sizeof(metadata_type_finder));

    switch(metadata_type_finder.type)
    {
        case META_TYPE_PREFERRED_AUDIO_CONTEXTS: ;
            metadata_preferred_audio_contexts meta_pac;
            memcpy(&meta_pac, _raw_metadata, sizeof(meta_pac));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, meta_pac);
            return META_TYPE_PREFERRED_AUDIO_CONTEXTS;

        case META_TYPE_STREAMING_AUDIO_CONTEXTS: ;
            metadata_streaming_audio_contexts meta_sac;
            memcpy(&meta_sac, _raw_metadata, sizeof(meta_sac));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, meta_sac);
            return META_TYPE_STREAMING_AUDIO_CONTEXTS;

        case META_TYPE_PROGRAM_INFO: ;
            metadata_program_info meta_pi;
            memcpy(&meta_pi, _raw_metadata, sizeof(meta_pi));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, meta_pi);
            return META_TYPE_PROGRAM_INFO;

        case META_TYPE_LANGUAGE: ;
            metadata_language meta_l;
            memcpy(&meta_l, _raw_metadata, sizeof(meta_l));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, meta_l);
            return META_TYPE_LANGUAGE;

        case META_TYPE_CCID_LIST: ;
            metadata_ccid_list meta_cl;
            memcpy(&meta_cl, _raw_metadata, sizeof(meta_cl));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, meta_cl);
            return META_TYPE_CCID_LIST;

        case META_TYPE_PARENTAL_RATING: ;
            metadata_parental_rating meta_pr;
            memcpy(&meta_pr, _raw_metadata, sizeof(meta_pr));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, meta_pr);
            return META_TYPE_PARENTAL_RATING;
            
        case META_TYPE_PROGRAM_INFO_URI: ;
            metadata_program_info_uri meta_pii;
            memcpy(&meta_pii, _raw_metadata, sizeof(meta_pii));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, meta_pii);
            return META_TYPE_PROGRAM_INFO_URI;

        case META_TYPE_EXTENDED_METADATA: ;
            metadata_extended_metadata meta_em;
            memcpy(&meta_em, _raw_metadata, sizeof(meta_em));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, meta_em);
            return META_TYPE_EXTENDED_METADATA;

        case META_TYPE_VENDOR_SPECIFIC: ;
            metadata_vendor_specific meta_vs;
            memcpy(&meta_vs, _raw_metadata, sizeof(meta_vs));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, meta_vs);
            return META_TYPE_VENDOR_SPECIFIC;

        case META_TYPE_AUDIO_ACTIVE_STATE: ;
            metadata_audio_active_state meta_aas;
            memcpy(&meta_aas, _raw_metadata, sizeof(meta_aas));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, meta_aas);
            return META_TYPE_AUDIO_ACTIVE_STATE;

        case META_TYPE_BROADCAST_ADUIO_IMMEDIATE_RENDERING_FLAG: ;
            metadata_broadcast_audio_immediate_rendering_flag meta_bairf;
            memcpy(&meta_bairf, _raw_metadata, sizeof(meta_bairf));
            net_buf_simple_add_u8(_buf, meta_bairf.length);
            net_buf_simple_add_u8(_buf, meta_bairf.type);
            return META_TYPE_BROADCAST_ADUIO_IMMEDIATE_RENDERING_FLAG;
        default: return -1;
    }

    return 0;
}

int extractCodecSpecificConfiguration(void *_raw_codec_specific_configuration, struct net_buf_simple *_buf)
{
    codec_specific_configuration_empty csc_type_finder;
    memcpy(&csc_type_finder, _raw_codec_specific_configuration, sizeof(csc_type_finder));

    switch(csc_type_finder.type)
    {
        case CSC_TYPE_SAMPLING_FREQUENCY: ;
            codec_specific_configuration_sampling_frequency csc_sf;
            memcpy(&csc_sf, _raw_codec_specific_configuration, sizeof(csc_sf));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, csc_sf);
            return CSC_TYPE_SAMPLING_FREQUENCY;

        case CSC_TYPE_FRAME_DURATION: ;
            codec_specific_configuration_frame_duration csc_fd;
            memcpy(&csc_fd, _raw_codec_specific_configuration, sizeof(csc_fd));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, csc_fd);
            return CSC_TYPE_FRAME_DURATION;

        case CSC_TYPE_AUDIO_CHANNEL_ALLOCATION: ;
            codec_specific_configuration_audio_channel_allocation csc_aca;
            memcpy(&csc_aca, _raw_codec_specific_configuration, sizeof(csc_aca));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, csc_aca);
            return CSC_TYPE_AUDIO_CHANNEL_ALLOCATION;

        case CSC_TYPE_OCTETS_PER_CODEC_FRAME: ;
            codec_specific_configuration_octets_per_codec_frame csc_opcf;
            memcpy(&csc_opcf, _raw_codec_specific_configuration, sizeof(csc_opcf));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, csc_opcf);
            return CSC_TYPE_OCTETS_PER_CODEC_FRAME;

        case CSC_TYPE_CODEC_FRAME_BLOCKS_PER_SDU: ;
            codec_specific_configuration_codec_frame_blocks_per_sdu csc_cfbps;
            memcpy(&csc_cfbps, _raw_codec_specific_configuration, sizeof(csc_cfbps));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, csc_cfbps);
            return CSC_TYPE_CODEC_FRAME_BLOCKS_PER_SDU;
        default: return -1;
    }

    return 0;
}

int extractCodecSpecificCapabilities(void *_raw_codec_specific_capabilities, struct net_buf_simple *_buf)
{
    codec_specific_capabilities_empty csc_type_finder;
    memcpy(&csc_type_finder, _raw_codec_specific_capabilities, sizeof(csc_type_finder));

    switch(csc_type_finder.type)
    {
        case CODEC_SPECIFIC_CAPABILITY_TYPE_SUPPORTED_SAMPLING_FREQUENCY: ;
            codec_specific_capabilities_supported_sampling_frequencies csc_sf;
            memcpy(&csc_sf, _raw_codec_specific_capabilities, sizeof(csc_sf));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, csc_sf);
            return CODEC_SPECIFIC_CAPABILITY_TYPE_SUPPORTED_SAMPLING_FREQUENCY;

        case CODEC_SPECIFIC_CAPABILITY_TYPE_SUPPORTED_FRAME_DURATION: ;
            codec_specific_capabilities_supported_frame_durations csc_fd;
            memcpy(&csc_fd, _raw_codec_specific_capabilities, sizeof(csc_fd));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, csc_fd);
            return CODEC_SPECIFIC_CAPABILITY_TYPE_SUPPORTED_FRAME_DURATION;

        case CODEC_SPECIFIC_CAPABILITY_TYPE_SUPPORTED_AUDIO_CHANNEL_COUNTS: ;
            codec_specific_capabilities_supported_audio_channel_counts csc_aca;
            memcpy(&csc_aca, _raw_codec_specific_capabilities, sizeof(csc_aca));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, csc_aca);
            return CODEC_SPECIFIC_CAPABILITY_TYPE_SUPPORTED_AUDIO_CHANNEL_COUNTS;

        case CODEC_SPECIFIC_CAPABILITY_TYPE_SUPPORTED_OCTETS_PER_CODEC_FRAME: ;
            codec_specific_capabilities_supported_octets_per_codec_frame csc_opcf;
            memcpy(&csc_opcf, _raw_codec_specific_capabilities, sizeof(csc_opcf));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, csc_opcf);
            return CODEC_SPECIFIC_CAPABILITY_TYPE_SUPPORTED_OCTETS_PER_CODEC_FRAME;

        case CODEC_SPECIFIC_CAPABILITY_TYPE_SUPPORTED_MAX_CODEC_FRAMES_PER_SDU: ;
            codec_specific_capabilities_supported_max_codec_frames_per_sdu csc_cfbps;
            memcpy(&csc_cfbps, _raw_codec_specific_capabilities, sizeof(csc_cfbps));
            ADD_AUDIO_STRUCT_TO_BUF(_buf, csc_cfbps);
            return CODEC_SPECIFIC_CAPABILITY_TYPE_SUPPORTED_MAX_CODEC_FRAMES_PER_SDU;

        default: return -1;
    }

    return 0;
}