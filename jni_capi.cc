#include <jni.h>
#include "capi.h"
#include <stdio.h>
#include <stdlib.h>


#define JNI_DEC_FUNC(v, p) JNIEXPORT v JNICALL Java_com_zenvv_capi_ADecoder_ ## p
#define JNI_ENC_FUNC(v, p) JNIEXPORT v JNICALL Java_com_zenvv_capi_AEncoder_ ## p


enum {
    kWebRtcOpusMaxDecodeFrameSizeMs = 120,
    kWebRtcOpusMaxFrameSizePerChannel = 48 * kWebRtcOpusMaxDecodeFrameSizeMs,
    kWebRtcOpusMaxFrameSize = kWebRtcOpusMaxFrameSizePerChannel * 2,
    kWebRtcOpusDefaultFrameSize = 48 * 20,

    kWebRtcAudioMaxFrameSize = kWebRtcOpusMaxFrameSize + 1000, // int16 each point.
};

jbyteArray as_jbyte_array(JNIEnv *env, unsigned char *buf, int len) {
    jbyteArray array = env->NewByteArray(len);
    if (array != NULL) {
        env->SetByteArrayRegion(array, 0, len, (jbyte *) (buf));
    }
    return array;
}

unsigned char *as_cbyte_array(JNIEnv *env, jbyteArray array, int *outlen) {
    if (!array) return NULL;
    int len = env->GetArrayLength(array);
    unsigned char *buf = (unsigned char *) malloc(len * sizeof(unsigned char));
    env->GetByteArrayRegion(array, 0, len, (jbyte *) (buf));
    if (outlen) {
        *outlen = len;
    }
    return buf;
}

short *as_cshort_array(JNIEnv *env, jshortArray array, int *outlen) {
    int len = env->GetArrayLength(array);
    short *buf = (short *) malloc(len * sizeof(short));
    env->GetShortArrayRegion(array, 0, len, (jshort *) (buf));
    if (outlen) {
        *outlen = len;
    }
    return buf;
}

extern "C" {

void Java_Jni_Test() {}

JNI_DEC_FUNC(jlong, CreateDecoder)(JNIEnv *env, jobject inst, jint codecId, jint nChannels)
{
    audio_codec_handle_t handle = create_audio_decoder(codecId, nChannels);
    return (jlong) handle;
}

JNI_DEC_FUNC(void, DestroyDecoder)(JNIEnv *env, jobject inst, jlong handle)
{
    audio_codec_handle_t codec = (audio_codec_handle_t) handle;
    destroy_audio_decoder(codec);
}

JNI_DEC_FUNC(jbyteArray, DecodeFrame)(JNIEnv *env, jobject inst, jlong handle, jbyteArray encoded, jboolean bFec)
{
    audio_codec_handle_t codec = (audio_codec_handle_t)handle;
    if (bFec) {
        int codec_id = get_audio_codec_id(codec);
        if (codec_id != OPUS_CODEC) {
            return NULL;
        }
    }

    int encoded_len = 0;
    unsigned char *p_encoded = as_cbyte_array(env, encoded, &encoded_len);
    unsigned char p_decoded[kWebRtcAudioMaxFrameSize*2] = {0};

    jbyteArray result = NULL;
    int iret = -1;
    if (bFec)
        iret = decode_audio_fec(codec, p_encoded, encoded_len, (short *)p_decoded);
    else
        iret = decode_audio_frame(codec, p_encoded, encoded_len, (short *)p_decoded);
    if (iret > 0) {
        result = as_jbyte_array(env, p_decoded, iret * 2);
        set_audio_codec_error(codec, 0);
    } else {
        set_audio_codec_error(codec, iret);
    }
    free(p_encoded);

    return result;
}

JNI_DEC_FUNC(jbyteArray, DecodePlc)(JNIEnv *env, jobject inst, jlong handle)
{
    audio_codec_handle_t codec = (audio_codec_handle_t)handle;
    unsigned char p_decoded[kWebRtcAudioMaxFrameSize*2] = {0};

    jbyteArray result = NULL;
    int iret = decode_audio_plc(codec, (short *)p_decoded);
    if (iret > 0) {
        result = as_jbyte_array(env, p_decoded, iret * 2);
        set_audio_codec_error(codec, 0);
    } else {
        set_audio_codec_error(codec, iret);
    }
    return result;
}

JNI_DEC_FUNC(jint, GetError)(JNIEnv *env, jobject inst, jlong handle) {
    audio_codec_handle_t codec = (audio_codec_handle_t)handle;
    return get_audio_codec_error(codec);
}


/**
 * Encoder
 */

JNI_ENC_FUNC(jlong, CreateEncoder)(JNIEnv *env, jobject inst, jint codecId, jint nChannels)
{
    audio_codec_handle_t handle = create_audio_encoder(codecId, nChannels);
    return (jlong) handle;
}

JNI_ENC_FUNC(void, DestroyEncoder)(JNIEnv *env, jobject inst, jlong handle)
{
    audio_codec_handle_t codec = (audio_codec_handle_t) handle;
    destroy_audio_encoder(codec);
}

JNI_ENC_FUNC(jbyteArray, EncodeFrame)(JNIEnv *env, jobject inst, jlong handle, jbyteArray pcmdata)
{
    audio_codec_handle_t codec = (audio_codec_handle_t) handle;
    int raw_len = 0;
    unsigned char *p_raw = as_cbyte_array(env, pcmdata, &raw_len);
    unsigned char p_encoded[kWebRtcAudioMaxFrameSize*2] = {0};

    jbyteArray result = NULL;
    int iret = encode_audio_frame(codec, (int16_t *)p_raw, raw_len/2, p_encoded, sizeof(p_encoded));
    if (iret > 0) {
        result = as_jbyte_array(env, p_encoded, iret);
        set_audio_codec_error(codec, 0);
    } else {
        set_audio_codec_error(codec, iret);
    }
    free(p_raw);
    return result;
}

JNI_ENC_FUNC(jint, GetError)(JNIEnv *env, jobject inst, jlong handle) {
    audio_codec_handle_t codec = (audio_codec_handle_t)handle;
    return get_audio_codec_error(codec);
}

} // extern "C"
