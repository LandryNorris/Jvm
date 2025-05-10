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

#endif //PRIMITIVE_ARRAY_H
