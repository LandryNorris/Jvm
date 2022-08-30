#include <errno.h>
#include <stdio.h>
#include <classloader.h>
#include <executor.h>
#include <stackframe.h>
#include <garbagecollector.h>

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("no class file given.\n");
        return EINVAL;
    }

    const char* mainClass = argv[1];
    const char* classPath = argv[2];

    Executor* executor = createExecutor(classPath, mainClass);

    runMain(executor);

    freeExecutor(executor);
}