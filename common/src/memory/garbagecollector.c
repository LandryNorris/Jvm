//
// Created by landry on 8/29/22.
//

#include <malloc.h>
#include "memory/garbagecollector.h"

GarbageCollector* createGarbageCollector() {
    GarbageCollector* gc = malloc(sizeof(GarbageCollector));
    gc->memoryRegion = createMemoryRegion(100000);

    return gc;
}

int allocateNew(GarbageCollector* gc, uint32_t size) {
    return allocate(gc->memoryRegion, size);
}
