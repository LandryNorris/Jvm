//
// Created by landry on 8/29/22.
//

#include <malloc.h>
#include <string.h>
#include "memory/garbagecollector.h"
#include "memory/objheader.h"
#include "classloader/utf8utils.h"

int createObject(GarbageCollector* gc, ClassFile* classFile) {
    int index = allocateNew(gc, sizeof(ObjHeader) + classFile->size);
    ObjHeader* header = getValue(gc->memoryRegion, index);

    int fieldCount = classFile->fieldPool->size;
    header->class = classFile;
    header->size = classFile->size;
    // TODO(Landry): Account for static fields
    header->fieldCount = fieldCount;
    header->fields = malloc(sizeof(char*)*fieldCount);
    int offset = 0;
    for(int i = 0; i < fieldCount; i++) {
        // we only care about instance fields
        if (isFieldStatic(classFile->fieldPool->pool[i])) {
            continue;
        }

        int nameIndex = classFile->fieldPool->pool[i]->nameIndex;
        int descriptorIndex = classFile->fieldPool->pool[i]->descriptorIndex;
        UTF8* nameUtf = classFile->constantPool->pool[nameIndex-1]->constant->utf8;
        UTF8* descriptorUtf = classFile->constantPool->pool[descriptorIndex-1]->constant->utf8;
        char* descriptor = utf82cstring(descriptorUtf);
        ObjField* field = malloc(sizeof(ObjField));

        field->offset = offset;
        field->name = utf82cstring(nameUtf);
        header->fields[i] = field;

        offset += getSizeFromDescriptor(descriptor);
    }
    return index;
}

int getFieldIndex(ObjHeader* obj, char* field) {
    for(int i = 0; i < obj->fieldCount; i++) {
        if(strcmp(field, obj->fields[i]->name) == 0) {
            return i;
        }
    }
    return -1;
}

void setFieldValue32(ObjHeader* obj, char* field, int32_t value) {
    int fieldIndex = getFieldIndex(obj, field);
    if(fieldIndex == -1) {
        printf("Field %s is not a valid field in the class\n", field);
        return;
    }
    int offset = obj->fields[fieldIndex]->offset;
    *(int32_t*) &obj->data[offset] = value;
}

int32_t getFieldValue32(ObjHeader* obj, char* field) {
    int fieldIndex = getFieldIndex(obj, field);
    if(fieldIndex == -1) {
        printf("Field %s is not a valid field in the class\n", field);
        return 0;
    }
    int offset = obj->fields[fieldIndex]->offset;
    return *(int32_t*) &obj->data[offset];
}

/**
 * @param descriptor String descriptor of the type
 * @return size in bytes of the primitive or reference
 */
int getSizeFromDescriptor(char* descriptor) {
    if(strcmp(descriptor, "B") == 0 || strcmp(descriptor, "Z") == 0) return 1;
    if(strcmp(descriptor, "C") == 0 || strcmp(descriptor, "S") == 0) return 2;
    if(strcmp(descriptor, "I") == 0 || strcmp(descriptor, "F") == 0) return 4;
    if(strcmp(descriptor, "J") == 0 || strcmp(descriptor, "D") == 0) return 8;
    return 4;
}
