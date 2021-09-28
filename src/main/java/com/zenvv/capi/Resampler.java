package com.zenvv.capi;

public class Resampler {
    private long audioHandle = 0;

    public Resampler(int inFreq, int outFreq) {
        audioHandle = CreateResampler(inFreq, outFreq);
    }

    public void Close() {
        DestroyResampler(audioHandle);
    }

    public short[] Push(short[] samplesIn) {
        return PushSample(audioHandle, samplesIn);
    }


    private native long CreateResampler(int inFreq, int outFreq);
    private native void DestroyResampler(long handle);
    private native short[] PushSample(long handle, short[] samplesIn);

    static  {
        System.loadLibrary("jnicapi_acodec");
    }
}
