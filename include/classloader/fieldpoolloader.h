#ifndef _FIELD_POOL_LOADER_H
#define _FIELD_POOL_LOADER_H

#include<stdint.h>

typedef struct {
	uint16_t accessFlags;
	uint16_t nameIndex;
	uint16_t descriptorIndex;
	uint16_t attributeCount;
	Attribute* attributes[];
} FieldPoolItem;

typedef struct {
	uint16_t fieldsCount;
	FieldPoolItem* pool;
} FieldPool;

#endif
