//
// Created by landry on 8/29/22.
//

#ifndef JVM_PROJECT_OBJHEADER_H
#define JVM_PROJECT_OBJHEADER_H

#include "classfile.h"

typedef struct {
    ClassFile* class;
    int size;
    uint8_t data[];
} ObjHeader;

int createObject(GarbageCollector* gc, ClassFile* class);

#endif //JVM_PROJECT_OBJHEADER_H
