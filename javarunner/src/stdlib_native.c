#include <stdio.h>
#include "native.h"

void loadNativeStdLib() {
    loadSharedLibrary("libstdlib_native.so");
}
