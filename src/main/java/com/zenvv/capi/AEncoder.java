package com.zenvv.capi;

public class AEncoder {
    // Codec id
    public static final int AUDIO_UNKNOW = 0;
    public static final int PCMA_CODEC = 1;
    public static final int PCMU_CODEC = 2;
    public static final int OPUS_CODEC = 3;
    public static final int G722_CODEC = 4;

    // Encode Options
    public static final int AUDIO_ENCODER_BITRATE = 0;     // [0,512*1024]bps. (default 64kbps)
    public static final int AUDIO_ENCODER_LOSSRATE = 1;    // [0-100], suggest <15. (default 5: 5%)
    public static final int AUDIO_ENCODER_FEC = 2;         // 0: disable, !=0: enable. (default enable)
    public static final int AUDIO_ENCODER_COMPLEXITY = 3;  // [0,10], suggest >=5. (default 10)


    private long audioHandle = 0;

    public AEncoder(int codecId) {
        audioHandle = CreateEncoder(codecId, 1);
    }

    public AEncoder(int codecId, int nChannels) {
        audioHandle = CreateEncoder(codecId, nChannels);
    }

    public void Close() {
        DestroyEncoder(audioHandle);
    }

    public byte[] Encode(byte[] encoded) {
        return EncodeFrame(audioHandle, encoded);
    }

    public int GetLastError() {
        return GetError(audioHandle);
    }

    /**
     * only support opus
     */
    public int SetBitrate(int bps) {
        return SetEncoderOption(audioHandle, AUDIO_ENCODER_BITRATE, bps);
    }

    public int SetPacketLossrate(int lossrate) {
        return SetEncoderOption(audioHandle, AUDIO_ENCODER_LOSSRATE, lossrate);
    }

    public int SetFec(boolean enabled) {
        int value = enabled ? 1 : 0;
        return SetEncoderOption(audioHandle, AUDIO_ENCODER_FEC, value);
    }

    public int SetComplexity(int complexity) {
        return SetEncoderOption(audioHandle, AUDIO_ENCODER_COMPLEXITY, complexity);
    }

    private native long CreateEncoder(int codecId, int nChannels);
    private native void DestroyEncoder(long handle);
    private native int SetEncoderOption(long handle, int option, int value);
    private native byte[] EncodeFrame(long handle, byte[] pcmdata);
    private native int GetError(long handle);

    static  {
        System.loadLibrary("jnicapi_acodec");
    }
}
