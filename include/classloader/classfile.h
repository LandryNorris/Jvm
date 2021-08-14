#ifndef CLASSFILE_H
#define CLASSFILE_H

#include<stdint.h>

typedef struct {
	uint32_t magic;
	uint16_t minor;
	uint16_t major;
	ConstantPool* constantPool;
	uint16_t accessFlags;
	uint16_t thisClass;
	uint16_t superClass;
	InterfacePool* interfacePool;
	uint16_t fieldsCount;
} ClassFile;
#endif
