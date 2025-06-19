#include "classpool/classpool.h"

#include <classloader/utf8utils.h>
#include <malloc.h>
#include <string.h>
#include <utils/constantpoolhelper.h>

#include "classloader/classfileloader.h"

ClassPool* createClassPool() {
    ClassPool* classPool = malloc(sizeof(ClassPool));
    int numToAllocate = 10;
    classPool->classFiles = malloc(numToAllocate * sizeof(ClassFile*));
    classPool->size = 0;
    classPool->numAllocated = numToAllocate;
}

void freeClassPool(ClassPool* classPool) {
    for (int i = 0; i < classPool->size; i++) {
        freeClassFile(classPool->classFiles[i]);
    }
    free(classPool->classFiles);
    free(classPool);
}

void increaseSize(ClassPool* classPool, int newSize) {
    ClassFile** newContent = malloc(newSize * sizeof(ClassFile*));

    for (int i = 0; i < classPool->size; i++) {
        newContent[i] = classPool->classFiles[i];
    }

    classPool->classFiles = newContent;
    classPool->numAllocated = newSize;
}

void addClassFile(ClassPool* classPool, ClassFile* file) {
    classPool->classFiles[classPool->size++] = file;
}

ClassFile* addClass(ClassPool* classPool, ClassFile* classFile) {
    if (classPool->size + 1 > classPool->numAllocated) {
        int newSize = (int) (classPool->numAllocated * 1.5);
        increaseSize(classPool, newSize);
    }

    addClassFile(classPool, classFile);
    return classFile;
}
