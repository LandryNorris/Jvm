#include <errno.h>
#include <stdio.h>
#include <classloader.h>

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("no class file given.\n");
        return EINVAL;
    }

    const char* mainClass = argv[1];
    const char* classPath = argv[2];

    ClassLoader* classLoader = createClassLoader(classPath, mainClass);

    ClassFile* classFile = getClassFile(classLoader, "Averager");

    printf("Class for Averager is %p", classFile);



    freeClassLoader(classLoader);
}