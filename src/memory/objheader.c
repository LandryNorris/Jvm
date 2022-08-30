//
// Created by landry on 8/29/22.
//

#include "garbagecollector.h"
#include "objheader.h"

int createObject(GarbageCollector* gc, ClassFile* classFile) {
    int index = allocateNew(gc, sizeof(ObjHeader) + classFile->size);
    ObjHeader* header = getValue(gc->memoryRegion, index);

    header->class = classFile;
    header->size = classFile->size;
    return index;
}
