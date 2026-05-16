#ifndef JVM_PROJECT_EXECUTION_AREA_H
#define JVM_PROJECT_EXECUTION_AREA_H

#include "stack.h"

typedef struct {
    Stack8* stack;
} ExecutionArea;

#endif // JVM_PROJECT_EXECUTION_AREA_H
