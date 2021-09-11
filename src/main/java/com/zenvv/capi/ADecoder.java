package com.zenvv.capi;

public class ADecoder {
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
