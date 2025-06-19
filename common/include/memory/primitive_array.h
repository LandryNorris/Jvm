#ifndef PRIMITIVE_ARRAY_H
#define PRIMITIVE_ARRAY_H

#include <stdint.h>

#include "garbagecollector.h"

typedef enum {
    T_BOOLEAN = 4,
    T_CHAR = 5,
    T_FLOAT = 6,
    T_DOUBLE = 7,
    T_BYTE = 8,
    T_SHORT = 9,
    T_INT = 10,
    T_LONG = 11,
} TypeCode;

typedef struct {
    TypeCode code;
    uint32_t length;
    uint8_t memory[];
} PrimitiveArray;

int createPrimitiveArray(GarbageCollector* gc, int type, uint32_t length);
void setCharArrayValue(GarbageCollector* gc, int obj, int index, uint16_t value);
uint16_t getCharArrayValue(GarbageCollector* gc, int obj, int index);

void setByteArrayValue(GarbageCollector* gc, int obj, int index, uint8_t value);
uint8_t getByteArrayValue(GarbageCollector* gc, int obj, int index);

uint32_t getArrayLength(GarbageCollector* gc, int obj);

#endif // PRIMITIVE_ARRAY_H
