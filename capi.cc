#include "capi.h"
#include "webrtc/common_audio/resampler/include/resampler.h"
#include "webrtc/modules/audio_coding/codecs/opus/interface/opus_interface.h"
#include "webrtc/modules/audio_coding/codecs/g711/include/g711_interface.h"
#include "webrtc/modules/audio_coding/codecs/g722/include/g722_interface.h"
#include <string.h>
#include <stdio.h>

#ifndef NULL
#define NULL (0)
#endif

#ifndef return_if_fail
#define return_if_fail(p) if (!(p)) {return;}
#define returnv_if_fail(p, v) if (!(p)) {return (v);}
#endif



/**
 * Audio ReSampler
 */

struct audio_resampler_t {
    webrtc::Resampler *ptr;
};

audio_resampler_handle_t create_audio_resampler(int inFreq, int outFreq) {
    audio_resampler_t *resampler = new audio_resampler_t;
    resampler->ptr = new webrtc::Resampler(inFreq, outFreq, webrtc::kResamplerSynchronous);
    return resampler;
}

void destroy_audio_resampler(audio_resampler_handle_t handle) {
    return_if_fail(handle);
    delete handle->ptr;
    delete handle;
}

int push_audio_resampler(audio_resampler_handle_t handle, int16_t* samplesIn, int lengthIn, int16_t* samplesOut, int maxLen, int *outLen) {
    returnv_if_fail(handle, -1);
    returnv_if_fail(handle->ptr, -1);

    webrtc::Resampler *resampler = handle->ptr;
    int outSampleLen = *outLen;
    int ret = resampler->Push(samplesIn, lengthIn, samplesOut, maxLen, outSampleLen);
    *outLen = outSampleLen;
    return ret;
}


/**
 * Audio Codec
 */ 

struct audio_codec_t {
    int id;
    int is_dec;
    void *ptr;
};

static uint32_t kDefaultCodecPtr = 0xabcd1234;


/**
 * Audio Decoder
 */ 

audio_codec_handle_t create_audio_decoder(int codec_id, int channels) {
    void *vptr = NULL;
    switch(codec_id) {
    case PCMA_CODEC:
    case PCMU_CODEC:
        vptr = (void *)&kDefaultCodecPtr;
        break;
    case OPUS_CODEC:
        {
            OpusDecInst *dptr = NULL;
            if (WebRtcOpus_DecoderCreate(&dptr, int(channels)) == 0) {
                if (WebRtcOpus_DecoderInit(dptr) == 0) {
                    vptr = (void *)dptr;
                } else {
                    WebRtcOpus_DecoderFree(dptr);
                }
            }
        }
        break;
    case G722_CODEC:
        {
            G722DecInst * dptr = NULL;
            if (WebRtcG722_CreateDecoder(&dptr) == 0) {
                if (WebRtcG722_DecoderInit(dptr) == 0) {
                    vptr = (void *)dptr;
                } else {
                    WebRtcG722_FreeDecoder(dptr);
                }
            }
        }
        break;
    default:
        break;
    }

    if (vptr) {
        audio_codec_t *codec = new audio_codec_t;
        memset(codec, 0, sizeof(audio_codec_t));
        codec->id = codec_id;
        codec->is_dec = true;
        codec->ptr = vptr;
        return codec;
    } else {
        return NULL;
    }
}

void destroy_audio_decoder(audio_codec_handle_t handle) {
    return_if_fail(handle);
    return_if_fail(handle->ptr);
    return_if_fail(handle->is_dec);

    switch(handle->id) {
    case PCMA_CODEC:
    case PCMU_CODEC:
        break;
    case OPUS_CODEC:
        {
            OpusDecInst *dptr = (OpusDecInst *)handle->ptr;
            WebRtcOpus_DecoderFree(dptr);
        }
        break;
    case G722_CODEC:
        {
            G722DecInst * dptr = (G722DecInst *)handle->ptr;
            WebRtcG722_FreeDecoder(dptr);
        }
        break;
    }

    handle->ptr = NULL;
    delete handle;
}

int decode_audio_frame(audio_codec_handle_t handle, uint8_t encoded[], int16_t encoded_len, int16_t decoded[]) {
    returnv_if_fail(handle, -1);
    returnv_if_fail(handle->ptr, -1);
    returnv_if_fail(handle->is_dec, -1);

    int16_t length = -1;
    int16_t temp_type = 0;
    switch(handle->id) {
    case PCMA_CODEC:
        length = WebRtcG711_DecodeA(NULL, (int16_t *)encoded, encoded_len, decoded, &temp_type);
        break;
    case PCMU_CODEC:
        length = WebRtcG711_DecodeU(NULL, (int16_t *)encoded, encoded_len, decoded, &temp_type);
        break;
    case OPUS_CODEC:
        {
            OpusDecInst *dptr = (OpusDecInst *)handle->ptr;
            length = WebRtcOpus_Decode(dptr, (int16_t *)encoded, encoded_len, decoded, &temp_type);
        }
        break;
    case G722_CODEC:
        {
            G722DecInst * dptr = (G722DecInst *)handle->ptr;
            length = WebRtcG722_Decode(dptr, (int16_t *)encoded, encoded_len, decoded, &temp_type);
        }
        break;
    }

    return length;
}

int decode_audio_fec(audio_codec_handle_t handle, uint8_t encoded[], int16_t encoded_len, int16_t decoded[]) {
    returnv_if_fail(handle, -1);
    returnv_if_fail(handle->ptr, -1);
    returnv_if_fail(handle->is_dec, -1);

    int16_t length = -1;
    if (handle->id == OPUS_CODEC) {
        OpusDecInst *dptr = (OpusDecInst *)handle->ptr;
        int16_t temp_type = 0;
        length = WebRtcOpus_DecodeFec(dptr, encoded, encoded_len, decoded, &temp_type);
    }
    return length;
}

int decode_audio_plc(audio_codec_handle_t handle, int16_t decoded[]) {
    returnv_if_fail(handle, -1);
    returnv_if_fail(handle->ptr, -1);
    returnv_if_fail(handle->is_dec, -1);

    int16_t length = -1;
    if (handle->id == OPUS_CODEC) {
        OpusDecInst *dptr = (OpusDecInst *)handle->ptr;
        length = WebRtcOpus_DecodePlc(dptr, decoded, 1);
    }
    return length;
}


/**
 * Audio Encoder
 */ 

audio_codec_handle_t create_audio_encoder(int codec_id, int channels) {
    void *vptr = NULL;
    switch(codec_id) {
    case PCMA_CODEC:
    case PCMU_CODEC:
        vptr = (void *)&kDefaultCodecPtr;
        break;
    case OPUS_CODEC:
        {
            OpusEncInst *eptr = NULL;
            if (WebRtcOpus_EncoderCreate(&eptr, channels) == 0) {
                WebRtcOpus_SetBitRate(eptr, 48000);
                WebRtcOpus_EnableFec(eptr);
                WebRtcOpus_SetPacketLossRate(eptr, 5);
                vptr = (void *)eptr;
            }
        }
        break;
    case G722_CODEC:
        {
            G722EncInst *eptr = NULL;
            if (WebRtcG722_CreateEncoder(&eptr) == 0) {
                if (WebRtcG722_EncoderInit(eptr) == 0) {
                    vptr = (void *)eptr;
                } else {
                    WebRtcG722_FreeEncoder(eptr);
                }
            }
        }
        break;
    }

    if (vptr) {
        audio_codec_t *codec = new audio_codec_t;
        memset(codec, 0, sizeof(audio_codec_t));
        codec->id = codec_id;
        codec->is_dec = false;
        codec->ptr = vptr;
        return codec;
    } else {
        return NULL;
    }
}

void destroy_audio_encoder(audio_codec_handle_t handle) {
    return_if_fail(handle);
    return_if_fail(handle->ptr);
    return_if_fail(!handle->is_dec);

    switch(handle->id) {
    case PCMA_CODEC:
    case PCMU_CODEC:
        break;
    case OPUS_CODEC:
        {
            OpusEncInst *eptr = (OpusEncInst *)handle->ptr;
            WebRtcOpus_EncoderFree(eptr);
        }
        break;
    case G722_CODEC:
        {
            G722EncInst * eptr = (G722EncInst *)handle->ptr;
            WebRtcG722_FreeEncoder(eptr);
        }
        break;
    }

    handle->ptr = NULL;
    delete handle;
}

int encode_audio_frame(audio_codec_handle_t handle, int16_t rawdata[], int16_t rawlength, uint8_t encoded[], int16_t encoded_buffer_length) {
    returnv_if_fail(handle, -1);
    returnv_if_fail(handle->ptr, -1);
    returnv_if_fail(!handle->is_dec, -1);

    int16_t length = -1;
    switch(handle->id) {
    case PCMA_CODEC:
        length = WebRtcG711_EncodeA(NULL, rawdata, rawlength, (int16_t *)encoded);
        break;
    case PCMU_CODEC:
        length = WebRtcG711_EncodeU(NULL, rawdata, rawlength, (int16_t *)encoded);
        break;
    case OPUS_CODEC:
        {
            OpusEncInst * eptr = (OpusEncInst *)handle->ptr;
            int16_t encoded_max_size = encoded_buffer_length;
            length = WebRtcOpus_Encode(eptr, rawdata, rawlength, encoded_max_size, encoded);
        }
        break;
    case G722_CODEC:
        {
            G722EncInst * eptr = (G722EncInst *)handle->ptr;
            length = WebRtcG722_Encode(eptr, rawdata, rawlength, (int16_t *)encoded);
        }
        break;
    }

    return length;
}

