#include "synthetic/class_creator.h"

#include <stdlib.h>
#include <string.h>

#include "classloader/utf8utils.h"

UTF8* addStringToConstantPool(ConstantPool* pool, int* index, const char* text) {
    UTF8* utf = malloc(sizeof(UTF8));
    char* copy = malloc(strlen(text) + 1);
    strcpy(copy, text);
    initUtf8(utf, copy);
    Constant* constant = malloc(sizeof(Constant));
    constant->utf8 = utf;
    pool->pool[*index]->tag = CONSTANT_UTF8;
    pool->pool[*index]->constant = constant;
    *index = *index + 1;

    return utf;
}

void addClassToConstantPool(ConstantPool* pool, int* index, Class* clazz) {
    Constant* constant = malloc(sizeof(Constant));
    constant->class = clazz;
    pool->pool[*index]->tag = CONSTANT_CLASS;
    pool->pool[*index]->constant = constant;
    *index = *index + 1;
}

ClassFile* createClassFile(const ClassCreationContext* context) {
    ClassFile* classFile = malloc(sizeof(ClassFile));
    memset(classFile, 0, sizeof(ClassFile));

    classFile->magic = 0xcafebabe;
    classFile->major = 52;
    classFile->minor = 0;

    // this class, this class name, superclass name, native methods
    int numConstants = 3 + 2*context->numNativeMethods;

    classFile->constantPool = malloc(sizeof(ConstantPool));
    classFile->constantPool->size = numConstants;

    classFile->constantPool->pool = malloc(numConstants * sizeof(ConstantPoolInfo*));
    for (int i = 0; i < numConstants; i++) {
        ConstantPoolInfo* info = malloc(sizeof(ConstantPoolInfo));
        classFile->constantPool->pool[i] = info;
    }

    int poolIndex = 0;
    Class* clazz = malloc(sizeof(Class));
    addClassToConstantPool(classFile->constantPool, &poolIndex, clazz);
    classFile->thisClassIndex = poolIndex;
    clazz->classFile = classFile;

    clazz->name = addStringToConstantPool(classFile->constantPool, &poolIndex, context->name);
    clazz->nameIndex = poolIndex;

    // initialize method pool
    classFile->methodPool = malloc(sizeof(MethodPool));
    classFile->methodPool->size = context->numNativeMethods;
    classFile->methodPool->pool = malloc(context->numNativeMethods * sizeof(MethodInfo*));

    for (int i = 0; i < context->numNativeMethods; i++) {
        NativeMethodCreationContext* method = &context->nativeMethods[i];

        MethodInfo* info = malloc(sizeof(MethodInfo));
        classFile->methodPool->pool[i] = info;

        info->name = addStringToConstantPool(classFile->constantPool, &poolIndex, method->name);
        info->descriptor = addStringToConstantPool(classFile->constantPool, &poolIndex, method->descriptor);

        info->accessFlags = METHOD_NATIVE | METHOD_SYNTHETIC;
        info->argumentCount = method->numArgs;
        info->attributePool = malloc(sizeof(AttributePool));
        info->attributePool->size = 0;
        info->attributePool->attributes = malloc(0);
    }

    // TODO(Landry): Attributes for class
    classFile->attributePool = malloc(sizeof(AttributePool));
    classFile->attributePool->size = 0;
    classFile->attributePool->attributes = malloc(0);

    classFile->fieldPool = malloc(sizeof(FieldPool));
    classFile->fieldPool->size = 0;
    classFile->fieldPool->pool = malloc(0);

    classFile->thisClass = clazz;
    classFile->name = clazz->name;

    if (context->superclass != nullptr) {
        const Class* superclass = context->superclass->constantPool->pool[context->superclass->thisClassIndex-1]->constant->class;
        UTF8* superclassNameUtf8 = context->superclass->constantPool->pool[superclass->nameIndex-1]->constant->utf8;

        char* superclassName = utf82cstring(superclassNameUtf8);
        addStringToConstantPool(classFile->constantPool, &poolIndex, superclassName);
        free(superclassName);
        classFile->superClassIndex = poolIndex;
    }

    return classFile;
}
