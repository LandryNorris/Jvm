#include <errno.h>
#include <stdio.h>
#include <execution_engine/executor.h>
#include "stdlib_native.h"

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("no class file given.\n");
        return EINVAL;
    }

    const char* mainClass = argv[1];
    const char* classPath = argv[2];

    Executor* executor = createExecutor(classPath, mainClass);
    setMainExecutor(executor);

    loadNativeStdLib();
    runMain(executor);

    freeExecutor(executor);
}