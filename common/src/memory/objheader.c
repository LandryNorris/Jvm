//
// Created by landry on 8/29/22.
//

#include "memory/objheader.h"

#include <malloc.h>
#include <string.h>

#include "classloader/utf8utils.h"
#include "execution_engine/executor.h"
#include "memory/garbagecollector.h"
#include "utils/log.h"

int getInstanceFieldCountOfClass(Executor* executor, ClassFile* classfile) {
    int numFieldsTotal = classfile->fieldPool->size;
    int numInstanceFields = 0;

    for (int i = 0; i < numFieldsTotal; i++) {
        if (!isFieldStatic(classfile->fieldPool->pool[i])) {
            numInstanceFields++;
        }
    }
    return numInstanceFields;
}

int getInstanceFieldCount(Executor* executor, ClassFile* classfile) {
    const Class* superclass = classfile->superClass;
    if (superclass == nullptr) {
        return getInstanceFieldCountOfClass(executor, classfile);
    }
    char* superclassName = utf82cstring(superclass->name);
    ClassFile* superclassFile = executor != nullptr ? getClassFileAndExecuteIfNew(executor, superclassName) : superclass->classFile;
    free(superclassName);
    return getInstanceFieldCountOfClass(executor, classfile) +
        classfile->superClass != nullptr ? getInstanceFieldCount(executor, superclassFile) : 0;
}

int createObject(Executor* executor, GarbageCollector* gc, ClassFile* classFile) {
    int index = allocateNew(gc, sizeof(ObjHeader) + classFile->size);
    ObjHeader* header = getValue(gc->memoryRegion, index);

    int fieldCount = getInstanceFieldCount(executor, classFile);
    header->class = classFile;
    header->size = classFile->size;
    // TODO(Landry): Account for static fields
    header->fieldCount = fieldCount;
    header->fields = malloc(sizeof(char*) * fieldCount);
    int offset = 0;
    for (int i = 0; i < fieldCount; i++) {
        // we only care about instance fields
        if (isFieldStatic(classFile->fieldPool->pool[i])) {
            continue;
        }

        UTF8* nameUtf = classFile->fieldPool->pool[i]->name;
        UTF8* descriptorUtf = classFile->fieldPool->pool[i]->descriptor;
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
    for (int i = 0; i < obj->fieldCount; i++) {
        if (strcmp(field, obj->fields[i]->name) == 0) {
            return i;
        }
    }
    return -1;
}

void setFieldValue32(ObjHeader* obj, char* field, int32_t value) {
    int fieldIndex = getFieldIndex(obj, field);
    if (fieldIndex == -1) {
        errorLog("Field %s is not a valid field in the class\n", field);
        return;
    }
    int offset = obj->fields[fieldIndex]->offset;
    *(int32_t*) &obj->data[offset] = value;
}

int32_t getFieldValue32(ObjHeader* obj, char* field) {
    int fieldIndex = getFieldIndex(obj, field);
    if (fieldIndex == -1) {
        errorLog("Field %s is not a valid field in the class\n", field);
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
    if (strcmp(descriptor, "B") == 0 || strcmp(descriptor, "Z") == 0) return 1;
    if (strcmp(descriptor, "C") == 0 || strcmp(descriptor, "S") == 0) return 2;
    if (strcmp(descriptor, "I") == 0 || strcmp(descriptor, "F") == 0 || descriptor[0] == '[')
        return 4;
    if (strcmp(descriptor, "J") == 0 || strcmp(descriptor, "D") == 0) return 8;
    return 4;
}
