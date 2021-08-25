#ifndef _ATTRIBUTE_LOADER_H
#define _ATTRIBUTE_LOADER_H

typedef struct {
	uint16_t nameIndex;
	uint32_t attributeLength;
	uint8_t* info;
} Attribute;

#endif
