#include <dlfcn.h>
#include <stdio.h>

static void* nativeStdlib;

void loadNativeStdLib() {
    nativeStdlib = dlopen("libstdlib_native.so", RTLD_NOW);

    printf("Loaded native stdlib");
}
