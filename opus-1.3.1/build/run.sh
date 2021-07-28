../configure \
    --host=mingw64 \
    --target=mingw64 \
    --disable-extra-programs \
    RANLIB=x86_64-w64-mingw32-ranlib \
    AR=x86_64-w64-mingw32-ar \
    CC=x86_64-w64-mingw32-gcc \
    LDFLAGS=" -lssp"
