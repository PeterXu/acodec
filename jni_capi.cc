#include <jni.h>
#include "capi.h"
#include <stdio.h>
#include <stdlib.h>


#define JNI_CPAI_FUNC(v, p) JNIEXPORT v JNICALL Java_com_zenvv_capi_ADecoder_ ## p

jbyteArray as_jbyte_array(JNIEnv *env, unsigned char *buf, int len) {
    jbyteArray array = env->NewByteArray(len);
    env->SetByteArrayRegion(array, 0, len, (jbyte *) (buf));
    return array;
}

unsigned char *as_cbyte_array(JNIEnv *env, jbyteArray array, int *outlen) {
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



JNI_CPAI_FUNC(jlong, CreateDecoder)(JNIEnv *env, jobject inst, jint codecId, jint nChannels)
{
    audio_codec_handle_t handle = create_audio_decoder(codecId, nChannels);
    return (jlong) handle;
}

JNI_CPAI_FUNC(void, DestroyDecoder)(JNIEnv *env, jobject inst, jlong handle)
{
    audio_codec_handle_t codec = (audio_codec_handle_t) handle;
    destroy_audio_decoder(codec);
}

JNI_CPAI_FUNC(jint, DecodeFrame)(JNIEnv *env, jobject inst, jlong handle, jbyteArray encoded, jshortArray decoded)
{
    audio_codec_handle_t codec = (audio_codec_handle_t)handle;
    int encoded_len = 0;
    unsigned char *p_encoded = as_cbyte_array(env, encoded, &encoded_len);
    short *p_decoded = as_cshort_array(env, decoded, NULL);
    int iret = decode_audio_frame(codec, p_encoded, encoded_len, p_decoded);
    return iret;
}

