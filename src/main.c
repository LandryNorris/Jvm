#include "classfile.h"
#include <stdio.h>
#include <errno.h>
#include <malloc.h>
#include <stack.h>
#include <string.h>
#include <stackframe.h>
#include <executor.h>

int loadClassFile(const char* classFilePath) {
    printf("Trying to open file at %s\n", classFilePath);
    FILE* file = fopen(classFilePath, "r");

    if(file == NULL) {
        printf("No such file\n");
        return ENOENT;
    }

    //Read length
    fseek(file, 0L, SEEK_END);
    long lengthBytes = ftell(file);
    fseek(file, 0L, SEEK_SET);

    uint8_t* content = malloc(lengthBytes+1);

    fread(content, lengthBytes, 1, file);

    ClassFile* classFilePtr = malloc(sizeof(ClassFile));
    initClassFile(content, classFilePtr);

    printClassFile(classFilePtr);

    FrameStack* frameStack = allocFrameStack(100);
    executeByName(classFilePtr, "main", frameStack);

    fclose(file);
    free(content);
    free(classFilePtr);

    return 0;
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("no class file given.\n");
        return EINVAL;
    }

    const char* classFilePath = argv[1];
    return loadClassFile(classFilePath);
}
