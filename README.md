
How to build in windows?

```
1. install mingw tools

2. build opus
    $> cd opus-1.3.1/build/
    $> sh run.sh
    $> make

3. build test
    $> make test

4. install jdk(cotains jni)
    $> WIN_JDK="windows/jdk/path" make

5. build jni-capi
    $> make testjni
    $> make jni

```


