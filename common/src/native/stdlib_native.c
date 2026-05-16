#include <stdio.h>

#include "native/native.h"

void loadNativeStdLib() {
    loadSharedLibrary("libstdlib_native.so");
}
