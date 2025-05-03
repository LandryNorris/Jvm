#ifndef JVM_PROJECT_METHODPARSER_H
#define JVM_PROJECT_METHODPARSER_H

#include<stdint.h>
#include "attributeloader.h"

typedef struct {
    uint16_t accessFlags;
    uint16_t nameIndex;
    uint16_t descriptorIndex;
    uint16_t argumentCount;
    AttributePool* attributePool;
} MethodInfo;

typedef struct {
    uint16_t size;
    MethodInfo** pool;
} MethodPool;

MethodPool* parseMethodPool(ConstantPool* constantPool, const uint8_t** content);

#endif
