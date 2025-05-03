//
// Created by landry on 8/29/22.
//

#include <stdint.h>
#include <malloc.h>
#include "memory/memory.h"

typedef struct {
    int index;
} ReservedData;

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
    memoryRegion->reserved = malloc(sizeof(ReservedData));
    ((ReservedData*)memoryRegion->reserved)->index = 1; //start at 1, so we can save 0 for null

    return memoryRegion;
}

/**
 * Allocates memory in a rolling pointer region
 * @param region
 * @param size size of memory to allocate in the region
 */
int allocate(MemoryRegion* region, uint32_t size) {
    if(((ReservedData*)region->reserved)->index + size >= region->size) return 0;
    int allocated = ((ReservedData*)region->reserved)->index;
    ((ReservedData*)region->reserved)->index += size;
    return allocated;
}

void* getValue(MemoryRegion* region, int index) {
    return region->memory + index;
}

void freeMemory(MemoryRegion* region, void* ptr) {
    //no-op on a rolling pointer region
}
