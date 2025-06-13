#ifndef _FIELD_POOL_LOADER_H
#define _FIELD_POOL_LOADER_H

#include<stdint.h>
#include "attributeloader.h"
#include "constantparser.h"

typedef enum {
	FIELD_PUBLIC = 0x1,
	FIELD_PRIVATE = 0x2,
	FIELD_PROTECTED = 0x4,
	FIELD_STATIC = 0x8,
	FIELD_FINAL = 0x10,
	FIELD_VOLATILE = 0x40,
	FIELD_TRANSIENT = 0x80,
	FIELD_SYNTHETIC = 0x1000,
	FIELD_ENUM = 0x4000,
} FieldAccessFlags;

typedef struct {
	uint16_t accessFlags;
	uint16_t nameIndex;
	uint16_t descriptorIndex;
	UTF8* name;
	UTF8* descriptor;
    AttributePool* attributePool;
} FieldPoolItem;

typedef struct {
	uint16_t size;
	FieldPoolItem** pool;
} FieldPool;

FieldPool* parseFieldPool(ConstantPool* constantPool, const uint8_t** content);

static bool isFieldStatic(const FieldPoolItem* field) {
	return field->accessFlags & FIELD_STATIC;
}

#endif
