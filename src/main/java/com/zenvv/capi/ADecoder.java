package com.zenvv.capi;

public class ADecoder {
    public static final int AUDIO_UNKNOW = 0;
    public static final int PCMA_CODEC = 1;
    public static final int PCMU_CODEC = 2;
    public static final int OPUS_CODEC = 3;
    public static final int G722_CODEC = 4;

    private long audioHandle = 0;

    public ADecoder(int codecId) {
        audioHandle = CreateDecoder(codecId, 1);
    }

    public ADecoder(int codecId, int nChannels) {
        audioHandle = CreateDecoder(codecId, nChannels);
    }

    public void Close() {
        DestroyDecoder(audioHandle);
    }

    public int Decode(byte[] encoded, byte[] decoded) {
        return DecodeFrame(audioHandle, encoded, decoded);
    }

    private native long CreateDecoder(int codecId, int nChannels);
    private native void DestroyDecoder(long handle);
    private native int DecodeFrame(long handle, byte[] encoded, byte[] decoded);

    static  {
        System.loadLibrary("jnicapi_acodec");
    }
}
