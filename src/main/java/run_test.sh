javac com/zenvv/capi/ADecoder.java
javac com/zenvv/capi/AEncoder.java
javac com/zenvv/capi/Resampler.java
javac TestJni.java

export LD_LIBRARY_PATH="../../../"
export DYLD_LIBRARY_PATH="../../../"
export JAVA_LIBRARY_PATH="../../../"
java TestJni

