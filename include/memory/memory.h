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
void* allocate(MemoryRegion* region, uint32_t size);
void freeMemory(MemoryRegion* region, void* ptr);

typedef struct {
    ClassFile* class;
    void* data;
} ObjHeader;

ObjHeader* createObject(ClassFile class);

#endif //JVM_PROJECT_MEMORY_H
