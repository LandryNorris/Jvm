//
// Created by landry on 8/29/22.
//

#include <malloc.h>
#include <constantpoolhelper.h>
#include <utf8utils.h>
#include <classpathhelper.h>
#include <classfileloader.h>
#include "classloader.h"

ClassLoader* createClassLoader(const char* classPath, const char* mainFilePath) {
    ClassLoader* classLoader = malloc(sizeof(ClassLoader));

    ClassPool* classPool = createClassPool();
    classLoader->mainClass = addClass(classPool, mainFilePath);

    classLoader->classPool = classPool;
    classLoader->classPath = classPath;
}

int indexOfClassFile(ClassLoader* classLoader, const char* classFileName) {
    for(int i = 0; i < classLoader->classPool->size; i++) {
        ClassFile* classFile = classLoader->classPool->classFiles[i];
        int index = classFile->thisClass;
        UTF8* utf8 = parseClassToUTF8ByIndex(index, classFile->constantPool);
        if(isEqual(utf8, classFileName)) return i;
    }
    return -1;
}

ClassFile* getClassFile(ClassLoader* classLoader, const char* classFileName) {
    int index = indexOfClassFile(classLoader, classFileName);

    if(index == -1) {
        const char* classPath = getPath(classLoader->classPath, classFileName);
        ClassFile* class = addClass(classLoader->classPool, classPath);
        return class;
    } else {
        return classLoader->classPool->classFiles[index];
    }
}

void freeClassLoader(ClassLoader* classLoader) {
    freeClassPool(classLoader->classPool);
    free(classLoader);
}
