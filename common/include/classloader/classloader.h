//
// Created by landry on 8/29/22.
//

#ifndef JVM_PROJECT_CLASSLOADER_H
#define JVM_PROJECT_CLASSLOADER_H

#include "classfile.h"
#include "classpool/classpool.h"

typedef struct {
    const char* classPath;
    const char* javaClassPath;
    const ClassFile* mainClass;
    ClassPool* classPool;
} ClassLoader;

ClassLoader* createClassLoader(const char* classPath, const char* javaClassPath, const char* mainFilePath);
ClassFile* getClassFile(ClassLoader* classLoader, const char* classFileName);
void freeClassLoader(ClassLoader* classLoader);

#endif //JVM_PROJECT_CLASSLOADER_H
