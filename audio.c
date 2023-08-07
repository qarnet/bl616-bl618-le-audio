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
            ADD_VARIED_ARR_TO_BUF(_buf, meta_pac);
            break;
        case META_TYPE_STREAMING_AUDIO_CONTEXTS: ;
            metadata_streaming_audio_contexts meta_sac;
            memcpy(&meta_sac, _raw_metadata, sizeof(meta_sac));
            ADD_VARIED_ARR_TO_BUF(_buf, meta_sac);
            break;
        case META_TYPE_PROGRAM_INFO: ;
            metadata_program_info meta_pi;
            memcpy(&meta_pi, _raw_metadata, sizeof(meta_pi));
            ADD_VARIED_POINTER_TO_BUF(_buf, meta_pi);
            break;
        case META_TYPE_LANGUAGE: ;
            metadata_language meta_l;
            memcpy(&meta_l, _raw_metadata, sizeof(meta_l));
            ADD_VARIED_ARR_TO_BUF(_buf, meta_l);
            break;
        case META_TYPE_CCID_LIST: ;
            metadata_ccid_list meta_cl;
            memcpy(&meta_cl, _raw_metadata, sizeof(meta_cl));
            ADD_VARIED_POINTER_TO_BUF(_buf, meta_cl);
            break;
        case META_TYPE_PARENTAL_RATING: ;
            metadata_parental_rating meta_pr;
            memcpy(&meta_pr, _raw_metadata, sizeof(meta_pr));
            ADD_VARIED_U8_TO_BUF(_buf, meta_pr);
            break;
        case META_TYPE_PROGRAM_INFO_URI: ;
            metadata_program_info_uri meta_pii;
            memcpy(&meta_pii, _raw_metadata, sizeof(meta_pii));
            ADD_VARIED_POINTER_TO_BUF(_buf, meta_pii);
            break;
        case META_TYPE_EXTENDED_METADATA: ;
            metadata_extended_metadata meta_em;
            memcpy(&meta_em, _raw_metadata, sizeof(meta_em));
            ADD_VARIED_POINTER_TO_BUF(_buf, meta_em);
            break;
        case META_TYPE_VENDOR_SPECIFIC: ;
            metadata_vendor_specific meta_vs;
            memcpy(&meta_vs, _raw_metadata, sizeof(meta_vs));
            ADD_VARIED_POINTER_TO_BUF(_buf, meta_vs);
            break;
        case META_TYPE_AUDIO_ACTIVE_STATE: ;
            metadata_audio_active_state meta_aas;
            memcpy(&meta_aas, _raw_metadata, sizeof(meta_aas));
            ADD_VARIED_U8_TO_BUF(_buf, meta_aas);
            break;
        case META_TYPE_BROADCAST_ADUIO_IMMEDIATE_RENDERING_FLAG: ;
            metadata_broadcast_audio_immediate_rendering_flag meta_bairf;
            memcpy(&meta_bairf, _raw_metadata, sizeof(meta_bairf));
            ADD_U8_TO_BUF(_buf, meta_bairf.length);
            ADD_U8_TO_BUF(_buf, meta_bairf.type);
            break;
        default: return -1;
    }

    return 0;
}

int extractCodecSpecificCapabilities(void *_raw_codec_specific_capabilities, struct net_buf_simple *_buf)
{
    codec_specific_configuration_empty csc_type_finder;
    memcpy(&csc_type_finder, _raw_codec_specific_capabilities, sizeof(csc_type_finder));

    switch(csc_type_finder.type)
    {
        case CSC_TYPE_SAMPLING_FREQUENCY: ;
            codec_specific_configuration_sampling_frequency csc_sf;
            memcpy(&csc_sf, _raw_codec_specific_capabilities, sizeof(csc_sf));
            ADD_VARIED_U8_TO_BUF(_buf, csc_sf);
            break;
        case CSC_TYPE_FRAME_DURATION: ;
            codec_specific_configuration_frame_duration csc_fd;
            memcpy(&csc_fd, _raw_codec_specific_capabilities, sizeof(csc_fd));
            ADD_VARIED_U8_TO_BUF(_buf, csc_sf);
            break;
        case CSC_TYPE_AUDIO_CHANNEL_ALLOCATION: ;
            codec_specific_configuration_audio_channel_allocation csc_aca;
            memcpy(&csc_aca, _raw_codec_specific_capabilities, sizeof(csc_aca));
            ADD_VARIED_U8_TO_BUF(_buf, csc_sf);
            break;
        case CSC_TYPE_OCTETS_PER_CODEC_FRAME: ;
            codec_specific_configuration_octets_per_codec_frame csc_opcf;
            memcpy(&csc_opcf, _raw_codec_specific_capabilities, sizeof(csc_opcf));
            ADD_VARIED_U8_TO_BUF(_buf, csc_sf);
            break;
        case CSC_TYPE_CODEC_FRAME_BLOCKS_PER_SDU: ;
            codec_specific_configuration_codec_frame_blocks_per_sdu csc_cfbps;
            memcpy(&csc_cfbps, _raw_codec_specific_capabilities, sizeof(csc_cfbps));
            ADD_VARIED_U8_TO_BUF(_buf, csc_sf);
            break;
        default: return -1;
    }

    return 0;
}