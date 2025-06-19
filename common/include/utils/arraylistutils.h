#ifndef JVM_PROJECT_ARRAYLISTUTILS_H
#define JVM_PROJECT_ARRAYLISTUTILS_H

#include <stdint.h>

typedef struct {
    int size;
    uint32_t currentIndex;
    char** values;
} CStringArrayList;
#endif // JVM_PROJECT_ARRAYLISTUTILS_H
