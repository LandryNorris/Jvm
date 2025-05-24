#include "memory/primitive_array.h"

#include <stdio.h>
#include <string.h>

#include "memory/garbagecollector.h"

int getElementSize(const int type) {
    switch (type) {
        case T_BOOLEAN:
        case T_BYTE: return 1;
        case T_CHAR:
        case T_SHORT: return 2;
        case T_INT:
        case T_FLOAT: return 4;
        case T_LONG:
        case T_DOUBLE: return 8;
        default: {
            printf("Invalid type: %d\n", type);
            return -1;
        }
    }
}

int createPrimitiveArray(GarbageCollector* gc, const int type, const uint32_t length) {
    const uint32_t arraySize = getElementSize(type) * length;
    const int index = allocateNew(gc, sizeof(PrimitiveArray) + arraySize);

    PrimitiveArray* array = getValue(gc->memoryRegion, index);
    array->length = length;
    array->code = (TypeCode) type;
    memset(&array->memory, 0, arraySize);

    return index;
}

void setCharArrayValue(GarbageCollector* gc, const int obj, const int index, const uint16_t value) {
    PrimitiveArray* array = getValue(gc->memoryRegion, obj);
    // TODO(Landry): Bounds checks
    // characters are two bytes in java
    const int dataIndex = index*2;
    array->memory[dataIndex] = value & 0xff;
    array->memory[dataIndex+1] = (value >> 8) & 0xff;
}

uint16_t getCharArrayValue(GarbageCollector* gc, const int obj, const int index) {
    PrimitiveArray* array = getValue(gc->memoryRegion, obj);
    // TODO(Landry): Bounds checks
    // characters are two bytes in java
    const int dataIndex = index*2;

    return array->memory[dataIndex+1] << 8 | array->memory[dataIndex];
}

uint32_t getArrayLength(GarbageCollector* gc, const int obj) {
    const PrimitiveArray* array = getValue(gc->memoryRegion, obj);
    // TODO(Landry): Bounds checks
    return array->length;
}
