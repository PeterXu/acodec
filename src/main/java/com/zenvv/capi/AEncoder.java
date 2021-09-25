package com.zenvv.capi;

public class AEncoder {
    public static final int AUDIO_UNKNOW = 0;
    public static final int PCMA_CODEC = 1;
    public static final int PCMU_CODEC = 2;
    public static final int OPUS_CODEC = 3;
    public static final int G722_CODEC = 4;

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

    /**
     * only support opus
     */
    public int SetBitrate(int bps) {
        return SetEncoderBitrate(audioHandle, bps);
    }

    public byte[] Encode(byte[] encoded) {
        return EncodeFrame(audioHandle, encoded);
    }

    public int GetLastError() {
        return GetError(audioHandle);
    }

    private native long CreateEncoder(int codecId, int nChannels);
    private native void DestroyEncoder(long handle);
    private native int SetEncoderBitrate(long handle, int bitrate);
    private native byte[] EncodeFrame(long handle, byte[] pcmdata);
    private native int GetError(long handle);

    static  {
        System.loadLibrary("jnicapi_acodec");
    }
}
