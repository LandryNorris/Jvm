#include "classloader/classfileloader.h"
#include <stdio.h>
#include <errno.h>

int javaparser_main(int argc, char** argv) {
    if(argc < 2) {
        printf("no class file given.\n");
        return EINVAL;
    }

    const char* classFilePath = argv[1];
    ClassFile* classFile = loadClassFile(classFilePath);

    printClassFile(classFile);
    freeClassFile(classFile);

    return 0;
}
