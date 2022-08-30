#include <errno.h>
#include <stdio.h>
#include <classloader.h>
#include <executor.h>
#include <stackframe.h>

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("no class file given.\n");
        return EINVAL;
    }

    const char* mainClass = argv[1];
    const char* classPath = argv[2];

    ClassLoader* classLoader = createClassLoader(classPath, mainClass);

    ClassFile* classFile = getClassFile(classLoader, "Averager");

    printf("Class for Averager is %p\n", classFile);

    FrameStack* frameStack = allocFrameStack(100);
    executeByName(classLoader->mainClass, "main", frameStack);

    freeClassLoader(classLoader);
}