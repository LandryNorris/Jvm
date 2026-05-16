//
// Created by landry on 8/29/22.
//

#ifndef JVM_PROJECT_OBJHEADER_H
#define JVM_PROJECT_OBJHEADER_H

#include "classloader/classfile.h"
#include "garbagecollector.h"
#include "execution_engine/executor.h"

typedef struct {
    ClassFile* class;
    FieldLayout* fieldLayout;
    uint8_t data[];
} ObjHeader;

int createObject(Executor* executor, GarbageCollector* gc, ClassFile* class);
void setFieldValue32(ObjHeader* obj, char* field, int32_t value);
int32_t getFieldValue32(ObjHeader* obj, char* field);

/**
 * @param descriptor String descriptor of the type
 * @return size in bytes of the primitive or reference
 */
int getSizeFromDescriptor(char* descriptor);

#endif // JVM_PROJECT_OBJHEADER_H
