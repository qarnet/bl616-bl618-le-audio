#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <stdint.h>
#include <buf.h>

int extractMetadata(void *_raw_metadata, struct net_buf_simple *_buf);
int extractCodecSpecificConfiguration(void *_raw_codec_specific_capabilities, struct net_buf_simple *_buf);
int extractCodecSpecificCapabilities(void *_raw_codec_specific_capabilities, struct net_buf_simple *_buf);

#endif