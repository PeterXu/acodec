package com.zenvv.capi;

public class Resampler {
    private long audioHandle = 0;

    /**
     * Mono Resampler: only supported some ratio(inFreq/outFreq).
     */
    public Resampler(int inFreq, int outFreq) {
        audioHandle = CreateResampler(inFreq, outFreq);
    }

    /**
     * Stereo Resampler
     */
    public Resampler(int inFreq, int outFreq, int channels) {
        audioHandle = CreateStereoResampler(inFreq, outFreq, channels);
    }

    public void Close() {
        DestroyResampler(audioHandle);
    }

    public short[] Push(short[] samplesIn) {
        return PushSample(audioHandle, samplesIn);
    }


    private native long CreateResampler(int inFreq, int outFreq);
    private native long CreateStereoResampler(int inFreq, int outFreq, int channels);
    private native void DestroyResampler(long handle);
    private native short[] PushSample(long handle, short[] samplesIn);

    static  {
        System.loadLibrary("jnicapi_acodec");
    }
}
