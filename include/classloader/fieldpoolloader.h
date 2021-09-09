#ifndef _FIELD_POOL_LOADER_H
#define _FIELD_POOL_LOADER_H

#include<stdint.h>
#include "attributeloader.h"
#include "constantparser.h"

typedef struct {
	uint16_t accessFlags;
	uint16_t nameIndex;
	uint16_t descriptorIndex;
    AttributePool* attributePool;
} FieldPoolItem;

typedef struct {
	uint16_t fieldsCount;
	FieldPoolItem** pool;
} FieldPool;

FieldPool* parseFieldPool(ConstantPool* constantPool, const uint8_t** content);

#endif
