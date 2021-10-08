#include <errno.h>
#include <stdio.h>
#include <classfile.h>
#include <dirent.h>
#include <classpool/classpool.h>
#include "classloader/classfileloader.h"

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("no class file given.\n");
        return EINVAL;
    }

    const char* mainClass = argv[1];
    const char* classFilePath = argv[2];

    ClassPool* classPool = loadClassPool(mainClass, classFilePath);

    freeClassPool(classPool);
}