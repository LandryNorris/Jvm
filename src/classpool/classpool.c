#include <malloc.h>
#include <string.h>
#include <utf8utils.h>
#include <constantpoolhelper.h>
#include "classpool.h"
#include "classfileloader.h"

ClassFile* findClassFileByName(ClassPool* classPool, UTF8* targetName) {
    for(int i = 0; i < classPool->size; i++) {
        UTF8* className = parseClassToUTF8ByIndex(classPool->classFiles[i]->thisClass, classPool->classFiles[i]->constantPool);

        if(isEqualUtf8(className, targetName)) {
            char* cstring = utf82cstring(className);
            printf("Found class with name %s\n", cstring);
            return classPool->classFiles[i];
        }
    }
    return NULL;
}

ClassPool* loadClassPool(const char* mainClass, const char* const classpath) {
    if(!mainClass) return NULL;

    ClassFile* mainClassFile = loadClassFile(mainClass);

    CStringArray* dependencies = getDependencyClasses(mainClassFile);

    ClassPool* classPool = malloc(sizeof(ClassPool));
    classPool->classFiles = malloc((1+dependencies->size)*sizeof(ClassFile*));
    classPool->size = 1+dependencies->size;

    classPool->classFiles[0] = mainClassFile;

    for(int i = 0; i < dependencies->size; i++) {
        char classFilePath[1000];
        strcpy(classFilePath, classpath);
        strcat(classFilePath, "/");
        strcat(classFilePath, dependencies->values[i]);
        strcat(classFilePath, ".class");
        classPool->classFiles[i + 1] = loadClassFile(classFilePath);
    }

    for(int i = 0; i < dependencies->size+1; i++) {
        ClassFile* classFile = classPool->classFiles[i];
        for(int j = 0; j < classFile->constantPool->size; j++) {
            ConstantPoolInfo* constantPoolInfo = classFile->constantPool->pool[j];
            if(constantPoolInfo == NULL) continue;
            if(constantPoolInfo->tag == CONSTANT_CLASS) {
                ClassFile* otherClass = findClassFileByName(classPool, parseClassToUTF8(constantPoolInfo->constant->class, classFile->constantPool));
                constantPoolInfo->constant->class->classFile = otherClass;
            }
        }
    }

    freeCStringArray(dependencies);

    return classPool;
}

void freeClassPool(ClassPool* classPool) {
    for(int i = 0; i < classPool->size; i++) {
        freeClassFile(classPool->classFiles[i]);
    }
    free(classPool->classFiles);
    free(classPool);
}