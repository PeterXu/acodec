package com.zenvv.capi;

public class ADecoder {
    public static final int AUDIO_UNKNOWN = 0;
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

    public byte[] Decode(byte[] encoded) {
        return DecodeFrame(audioHandle, encoded, 0);
    }

    public short[] DecodeEx(byte[] encoded) {
        return DecodeFrameEx(audioHandle, encoded, 0);
    }

    /**
     * only support opus
     */
    public byte[] DecodeFec(byte[] encoded) {
        return DecodeFrame(audioHandle, encoded, 1);
    }

    /**
     * only support opus
     */
    public byte[] DecodePlc() {
        return DecodeFrame(audioHandle, null, 2);
    }

    public int GetLastError() {
        return GetError(audioHandle);
    }

    private native long CreateDecoder(int codecId, int nChannels);
    private native void DestroyDecoder(long handle);
    private native byte[] DecodeFrame(long handle, byte[] encoded, int mode);
    private native short[] DecodeFrameEx(long handle, byte[] encoded, int mode);
    private native int GetError(long handle);

    static  {
        System.loadLibrary("jnicapi_acodec");
    }
}
