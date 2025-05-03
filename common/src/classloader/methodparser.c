#include "classloader/methodparser.h"
#include<stdint.h>
#include <malloc.h>
#include <classloader/primitivereader.h>

MethodPool* parseMethodPool(ConstantPool* constantPool, const uint8_t** content) {
    MethodPool* methodPool = malloc(sizeof(MethodPool));

    uint16_t size = readuInt16(content);
    methodPool->size = size;
    methodPool->pool = malloc(size * sizeof(MethodInfo*));

    for(int i = 0; i < size; i++) {
        MethodInfo* methodInfo = malloc(sizeof(MethodInfo));

        methodInfo->accessFlags = readuInt16(content);
        methodInfo->nameIndex = readuInt16(content);
        methodInfo->descriptorIndex = readuInt16(content);

        methodInfo->attributePool = parseAttributes(constantPool, content);
        methodPool->pool[i] = methodInfo;
    }
    return methodPool;
}