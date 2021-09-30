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
 * This is mono-resampler and supported inFreq and outFreq:
 *  a) inFreq == outFreq,
 *  b) outFreq / inFreq = 2|3|4|6|12,
 *  c) inFreq / outFreq = 2|3|4|6|12,
 *  d) inFreq : outFreq = 2:3, 2:11, 4:11, 8:11, 
 *  3) inFreq : outFreq = 3:2, 11:2, 11:4, 11:16, 11:32, 11:8
 *
 * inFreq [in]: input frequency, e.g. 32000,
 * outFreq [out]: output frequency, e.g. 16000,
 */
audio_resampler_handle_t create_audio_resampler(int inFreq, int outFreq);

/**
 * This is stereo resampler and support all kinds of frequency.
 * channels[in]: 1 or 2.
 */
audio_resampler_handle_t create_audio_stereo_resampler(int inFreq, int outFreq, int channels);

void destroy_audio_resampler(audio_resampler_handle_t handle);

/**
 * @return: fail if <=0, ok if > 0: the integer multiple of output/input,
 *          e.g. return-N means if input is 1, output will be [N-1, N).
 */
int get_audio_resampler_output_multiple(audio_resampler_handle_t handle);

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
 * set encode options, only valid for opus.
 * @return: 0 if ok, else -1.
 */
enum AudioEncoderOption {
    AUDIO_ENCODER_BITRATE = 0, // [0,512*1024]bps. (default 64kbps)
    AUDIO_ENCODER_LOSSRATE,    // [0-100], suggest <15. (default 5: 5%)
    AUDIO_ENCODER_FEC,         // 0: disable, !=0: enable. (default enable)
    AUDIO_ENCODER_COMPLEXITY,  // [0,10], suggest >=5. (default 10)
};
int set_audio_encoder_option(audio_codec_handle_t handle, AudioEncoderOption option, int value);

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
