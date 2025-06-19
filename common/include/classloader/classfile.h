#ifndef CLASSFILE_H
#define CLASSFILE_H

#include <stdint.h>

#include "constantparser.h"
#include "fieldpoolloader.h"
#include "interfaceparser.h"
#include "memory/static.h"
#include "methodparser.h"

// we predefined the ClassFile in constantparser.h
struct ClassFile {
    int size;
    uint32_t magic;
    uint16_t minor;
    uint16_t major;
    ConstantPool* constantPool;
    uint16_t accessFlags;
    uint16_t thisClassIndex;
    uint16_t superClassIndex;
    InterfacePool* interfacePool;
    FieldPool* fieldPool;
    MethodPool* methodPool;
    AttributePool* attributePool;

    Class* thisClass;
    Class* superClass;
    UTF8* name;
    struct StaticFields* staticFields;
};

int initClassFile(const uint8_t* bytes, ClassFile* classPtr);

void printClassFile(ClassFile* classFilePtr);
#endif
