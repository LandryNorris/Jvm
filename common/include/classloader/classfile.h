#ifndef CLASSFILE_H
#define CLASSFILE_H

#include "constantparser.h"
#include "interfaceparser.h"
#include "fieldpoolloader.h"
#include "methodparser.h"
#include <stdint.h>
#include "memory/static.h"

//we predefined the ClassFile in constantparser.h
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

int initClassFile(const uint8_t * bytes, ClassFile* classPtr);

void printClassFile(ClassFile* classFilePtr);
#endif
