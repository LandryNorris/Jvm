#include <errno.h>
#include <execution_engine/executor.h>
#include <stdio.h>

#include "stdlib_native.h"
#include "utils/log.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("no class file given.\n");
        return EINVAL;
    }

    const char* mainClass = argv[1];
    const char* classPath = argv[2];

    if (argc >= 3) {
        const char* logLevel = argv[3];
        const LogLevel level = getLogLevelByName(logLevel);
        if (level == -1) {
            printf("%s is not a valid log level\n", logLevel);
            return EINVAL;
        }
        setLogLevel(level);
    }

    Executor* executor = createExecutor(classPath, mainClass);
    setMainExecutor(executor);

    loadNativeStdLib();
    runMain(executor);

    freeExecutor(executor);
}
