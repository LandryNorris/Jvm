//
// Created by landry on 8/29/22.
//

#ifndef JVM_PROJECT_MEMORY_H
#define JVM_PROJECT_MEMORY_H

#include "classfile.h"

typedef struct {
    uint64_t size;
    void* memory;
    void* reserved;
} MemoryRegion;

MemoryRegion* createMemoryRegion(uint64_t size);
int allocate(MemoryRegion* region, uint32_t size);
void* getValue(MemoryRegion* region, int index);
void freeMemory(MemoryRegion* region, void* ptr);

#endif //JVM_PROJECT_MEMORY_H
