#ifndef _AUDIO_CPAI_H_
#define _AUDIO_CPAI_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Audio Resampler
 */

typedef struct audio_resampler_t *audio_resampler_handle_t;


/**
 * inFreq [in]: input frequency, e.g. 32000,
 * outFreq [out]: output frequency, e.g. 16000,
 */
audio_resampler_handle_t create_audio_resampler(int inFreq, int outFreq);
void destroy_audio_resampler(audio_resampler_handle_t handle);


/**
 * samplesIn [in]: input samples(pcm)
 * lengthIn [in]: the length of input samples,
 * samplesOut [out]: the outputing buffer, 
 * maxLen [in]: the length of outputing buffer,
 * outLen [out]: the length of outputing samples in buffer,
 * @return: 0 if ok, else -1
 */
int push_audio_resampler(audio_resampler_handle_t handle, int16_t* samplesIn, int lengthIn, int16_t* samplesOut, int maxLen, int *outLen);



/**
 * Audio Codec
 */

enum audio_codec_id_t {
    AUDIO_UNKNOWN = 0,
    PCMA_CODEC,
    PCMU_CODEC,
    OPUS_CODEC,
    G722_CODEC,
};

typedef struct audio_codec_t * audio_codec_handle_t;

/**
 * audio codec query/error
 */
int get_audio_codec_id(audio_codec_handle_t handle);
void set_audio_codec_error(audio_codec_handle_t handle, int error);
int get_audio_codec_error(audio_codec_handle_t handle);


/**
 * audio decoder
 */

audio_codec_handle_t create_audio_decoder(int codec_id, int channels);
void destroy_audio_decoder(audio_codec_handle_t handle);

/**
 * encoded [in]: the encoded audio data
 * encoded_len [in]: the length of encoded audio data
 * decoded [out]: the buffer for decoded sampels, (assure it enough)
 * @return: decoded length if ok, else -1.
 */
int decode_audio_frame(audio_codec_handle_t handle, uint8_t encoded[], int16_t encoded_len, int16_t decoded[]);

// only for opus
int decode_audio_fec(audio_codec_handle_t handle, uint8_t encoded[], int16_t encoded_len, int16_t decoded[]);
int decode_audio_plc(audio_codec_handle_t handle, int16_t decoded[]);



/**
 * audio encoder
 */
audio_codec_handle_t create_audio_encoder(int codec_id, int channels); 
void destroy_audio_encoder(audio_codec_handle_t handle);


/**
 * rawdata [in]: raw pcm data,
 * rawlength [in]: the length of raw pcm data,
 * encoded [out]: the buffer for encoded samples,(assure it enough)
 * encoded_buffer_length [in]: the buffer length for encoded samples,
 * @return: encoded length if ok, else -1.
 */
int encode_audio_frame(audio_codec_handle_t handle, int16_t rawdata[], int16_t rawlength, uint8_t encoded[], int16_t encoded_buffer_length);


#ifdef __cplusplus
}
#endif

#endif // _AUDIO_CPAI_H_
