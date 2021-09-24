
How to build in windows?

```
0. copy Makefile.mingw to Makefile

1. install mingw tools

2. build opus
    $> cd opus-1.3.1/build/
    $> sh run.sh
    $> make

3. build test
    $> make test
    $> make shared

4. install jdk(contains jni.h)
    $> WIN_JDK="windows/jdk/path" make

5. build jni-capi
    $> make testjni
    $> make jni

```


How to build in Linux?

```
0. $> cp Makefile.unix Makefile

1. install opus
    CentOS: $> yum install opus-devel
    Ubuntu: $> apt-get install libopus-dev

2. build test
    $> make test
    $> make shared

3. install java-jdk
    CentOS: $> yum install java-11-openjdk
    Ubuntu: $> apt-get install openjdk-11-jdk

4. build jni-capi
    $> make testjni
    $> make jni

```



The error of opus-codec

```
/** One or more invalid/out of range arguments @hideinitializer*/
#define OPUS_BAD_ARG          -1
/** Not enough bytes allocated in the buffer @hideinitializer*/
#define OPUS_BUFFER_TOO_SMALL -2
/** An internal error was detected @hideinitializer*/
#define OPUS_INTERNAL_ERROR   -3
/** The compressed data passed is corrupted @hideinitializer*/
#define OPUS_INVALID_PACKET   -4
/** Invalid/unsupported request number @hideinitializer*/
#define OPUS_UNIMPLEMENTED    -5
/** An encoder or decoder structure is invalid or already freed @hideinitializer*/
#define OPUS_INVALID_STATE    -6
/** Memory allocation has failed @hideinitializer*/
#define OPUS_ALLOC_FAIL       -7
```
