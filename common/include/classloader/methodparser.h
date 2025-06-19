#ifndef JVM_PROJECT_METHODPARSER_H
#define JVM_PROJECT_METHODPARSER_H

#include <stdint.h>

#include "attributeloader.h"

typedef struct {
    uint16_t accessFlags;
    uint16_t argumentCount;
    UTF8* name;
    UTF8* descriptor;
    AttributePool* attributePool;
} MethodInfo;

typedef struct {
    uint16_t size;
    MethodInfo** pool;
} MethodPool;

typedef enum {
    METHOD_PUBLIC = 0x01,
    METHOD_PRIVATE = 0x02,
    METHOD_PROTECTED = 0x04,
    METHOD_STATIC = 0x08,
    METHOD_FINAL = 0x10,
    METHOD_SYNCHRONIZED = 0x20,
    METHOD_BRIDGE = 0x40,
    METHOD_VARARGS = 0x80,
    METHOD_NATIVE = 0x100,
    METHOD_ABSTRACT = 0x400,
    METHOD_STRICT = 0x800,
    METHOD_SYNTHETIC = 0x1000,
} MethodAccessFlags;

static bool isNative(const MethodInfo* method) {
    return method->accessFlags & METHOD_NATIVE;
}

MethodPool* parseMethodPool(ConstantPool* constantPool, const uint8_t** content);

#endif
