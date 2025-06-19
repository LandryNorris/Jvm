//
// Created by landry on 8/29/22.
//

#include "classloader/classloader.h"

#include <classloader/classfileloader.h>
#include <classloader/utf8utils.h>
#include <malloc.h>
#include <utils/classpathhelper.h>
#include <utils/constantpoolhelper.h>

ClassLoader* createClassLoader(const char* classPath, const char* javaClassPath,
                               const char* mainFilePath) {
    ClassLoader* classLoader = malloc(sizeof(ClassLoader));

    ClassPool* classPool = createClassPool();
    ClassFile* mainClass = loadClassFile(mainFilePath);
    classLoader->mainClass = addClass(classPool, mainClass);

    classLoader->classPool = classPool;
    classLoader->classPath = classPath;
    classLoader->javaClassPath = javaClassPath;
}

int indexOfClassFile(ClassLoader* classLoader, const char* classFileName) {
    for (int i = 0; i < classLoader->classPool->size; i++) {
        ClassFile* classFile = classLoader->classPool->classFiles[i];
        UTF8* utf8 = classFile->name;
        if (isEqual(utf8, classFileName)) return i;
    }
    return -1;
}

ClassFile* getClassFile(ClassLoader* classLoader, const char* classFileName, uint8_t* loadedFresh) {
    int index = indexOfClassFile(classLoader, classFileName);

    if (index == -1) {
        const char* classPath = getPath(classLoader->classPath, classFileName);

        ClassFile* classFile = loadClassFile(classPath);
        if (!classFile) {
            const char* javaClassPath = getPath(classLoader->javaClassPath, classFileName);
            classFile = loadClassFile(javaClassPath);
        }
        ClassFile* class = addClass(classLoader->classPool, classFile);
        if (loadedFresh) {
            *loadedFresh = 1;
        }
        return class;
    } else {
        if (loadedFresh) {
            *loadedFresh = 0;
        }
        return classLoader->classPool->classFiles[index];
    }
}

void freeClassLoader(ClassLoader* classLoader) {
    freeClassPool(classLoader->classPool);
    free(classLoader);
}
