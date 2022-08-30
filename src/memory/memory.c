//
// Created by landry on 8/29/22.
//

#include <stdint.h>
#include <malloc.h>
#include "memory.h"

/**
 * Creates a MemoryRegion with a rolling pointer allocator for now. It's not efficient, but it works as
 * an initial solution.
 * @param size size of the memory region in bytes
 * @return
 */
MemoryRegion* createMemoryRegion(uint64_t size) {
    MemoryRegion* memoryRegion = malloc(sizeof(MemoryRegion));
    void* memory = malloc(size);

    memoryRegion->memory = memory;
    memoryRegion->size = size;
    memoryRegion->reserved = memory;

    return memoryRegion;
}

/**
 * Allocates memory in a rolling pointer region
 * @param region
 * @param size size of memory to allocate in the region
 */
void* allocate(MemoryRegion* region, uint32_t size) {
    if(region->reserved + size >= region->memory + region->size) return 0;
    void* allocated = region->reserved;
    region->reserved += size;
    return allocated;
}

void freeMemory(MemoryRegion* region, void* ptr) {
    //no-op on a rolling pointer region
}

ObjHeader* createObjectHeader(ClassFile* classFile) {

}
