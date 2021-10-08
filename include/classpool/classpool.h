#ifndef JVM_PROJECT_CLASSPOOL_H
#define JVM_PROJECT_CLASSPOOL_H

#include <classfile.h>

typedef struct {
    ClassFile** classFiles;
    int size;
} ClassPool;

ClassPool* loadClassPool(const char* mainClass, const char* classpath);

void freeClassPool(ClassPool* classPool);

#endif //JVM_PROJECT_CLASSPOOL_H
