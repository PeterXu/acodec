
How to build in windows?

```
1. install mingw tools

2. build opus
    $> cd opus-1.3.1/build/
    $> sh run.sh
    $> make

3. build test
    $> make test
    $> make shared

4. install jdk(cotains jni)
    $> WIN_JDK="windows/jdk/path" make

5. build jni-capi
    $> make testjni
    $> make jni

```


How to build in Linux?

```
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


