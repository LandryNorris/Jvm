#include <errno.h>
#include <stdio.h>
#include <classloader/classloader.h>
#include <execution_engine/executor.h>
#include <interpreter/stackframe.h>
#include <memory/garbagecollector.h>

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