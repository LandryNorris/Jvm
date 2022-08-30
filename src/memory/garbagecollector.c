//
// Created by landry on 8/29/22.
//

#include <malloc.h>
#include "garbagecollector.h"

GarbageCollector* createGarbageCollector() {
    GarbageCollector* gc = malloc(sizeof(GarbageCollector));
    gc->memoryRegion = createMemoryRegion(100000);

    return gc;
}

void* allocateNew(GarbageCollector* gc, uint32_t size) {
    allocate(gc->memoryRegion, size);
}
