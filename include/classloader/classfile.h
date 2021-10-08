#ifndef CLASSFILE_H
#define CLASSFILE_H

#include "constantparser.h"
#include "interfaceparser.h"
#include "fieldpoolloader.h"
#include "methodparser.h"
#include <stdint.h>
#include "constantparser.h"

//we predefined the ClassFile in constantparser.h
struct ClassFile {
	uint32_t magic;
	uint16_t minor;
	uint16_t major;
	ConstantPool* constantPool;
	uint16_t accessFlags;
	uint16_t thisClass;
	uint16_t superClass;
	InterfacePool* interfacePool;
    FieldPool* fieldPool;
    MethodPool* methodPool;
    AttributePool* attributePool;
};

int initClassFile(const uint8_t * bytes, ClassFile* classPtr);

void printClassFile(ClassFile* classFilePtr);
#endif
