#ifndef JVM_PROJECT_CLASSPOOL_H
#define JVM_PROJECT_CLASSPOOL_H

#include <classfile.h>

typedef struct {
    ClassFile** classFiles;
    int size;
    int numAllocated;
} ClassPool;

ClassPool* createClassPool();
ClassFile * addClass(ClassPool* classPool, ClassFile* classFile);
void freeClassPool(ClassPool* classPool);

#endif //JVM_PROJECT_CLASSPOOL_H
