package com.zenvv.capi;

public class ADecoder {
    public static final int AUDIO_UNKNOWN = 0;
    public static final int PCMA_CODEC = 1;
    public static final int PCMU_CODEC = 2;
    public static final int OPUS_CODEC = 3;
    public static final int G722_CODEC = 4;

    private int audioCodecId = 0;
    private long audioHandle = 0;

    public ADecoder(int codecId) {
        audioCodecId = codecId;
        audioHandle = CreateDecoder(codecId, 1);
    }

    public ADecoder(int codecId, int nChannels) {
        audioCodecId = codecId;
        audioHandle = CreateDecoder(codecId, nChannels);
    }

    public void Close() {
        DestroyDecoder(audioHandle);
    }

    public byte[] Decode(byte[] encoded) {
        return DecodeFrame(audioHandle, encoded, false);
    }

    public byte[] DecodeFec(byte[] encoded) {
        if (audioCodecId == OPUS_CODEC) {
            return DecodeFrame(audioHandle, encoded, true);
        } else {
            return null;
        }
    }

    public int GetLastError() {
        return GetError(audioHandle);
    }

    private native long CreateDecoder(int codecId, int nChannels);
    private native void DestroyDecoder(long handle);
    private native byte[] DecodeFrame(long handle, byte[] encoded, boolean bFec);
    private native int GetError(long handle);

    static  {
        System.loadLibrary("jnicapi_acodec");
    }
}
