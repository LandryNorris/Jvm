#ifndef STATIC_FIELDS_H
#define STATIC_FIELDS_H

struct StaticFields;

#include "classloader/classfile.h"

typedef struct {
    const char* name;
    uint64_t value;
} StaticField;

typedef struct StaticFields {
    ClassFile* class;
    int numFields;
    StaticField* fields;
} StaticFields;

void initializeStaticFields(ClassFile* classFile);

StaticField* getStaticField(ClassFile* classFile, const char* field);
int32_t getInt32StaticField(ClassFile* classFile, const char* field);
int32_t setInt32StaticField(ClassFile* classFile, const char* field, int32_t value);

#endif //STATIC_FIELDS_H
