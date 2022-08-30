//
// Created by landry on 8/29/22.
//

#ifndef JVM_PROJECT_GARBAGECOLLECTOR_H
#define JVM_PROJECT_GARBAGECOLLECTOR_H

#include <stdint.h>
#include "memory.h"

typedef struct {
    MemoryRegion* memoryRegion;
} GarbageCollector;

GarbageCollector* createGarbageCollector();
void* allocateNew(GarbageCollector* gc, uint32_t size);

#endif //JVM_PROJECT_GARBAGECOLLECTOR_H
