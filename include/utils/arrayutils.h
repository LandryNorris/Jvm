#include <constantparser.h>

#ifndef JVM_PROJECT_ARRAYUTILS_H
#define JVM_PROJECT_ARRAYUTILS_H

typedef struct {
    int size;
    void* values;
} Array;

typedef struct {
    int size;
    char** values;
} CStringArray;

Array* allocArray(int size, int itemSize);

CStringArray* allocCStringArray(int size);

void freeCStringArray(CStringArray* cStringArray);

int containsUtf8(CStringArray*, UTF8*);

#endif //JVM_PROJECT_ARRAYUTILS_H
