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

int getInstanceFieldCountOfClass(ClassFile* classfile) {
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
    Class* superclass = classfile->superClass;
    if (superclass == nullptr) {
        return getInstanceFieldCountOfClass(classfile);
    }
    char* superclassName = utf82cstring(superclass->name);
    ClassFile* superclassFile = executor != nullptr ? getClassFileAndExecuteIfNew(executor, superclassName) : superclass->classFile;
    superclass->classFile = superclassFile;
    free(superclassName);
    return getInstanceFieldCountOfClass(classfile) +
        (superclassFile != nullptr ? getInstanceFieldCount(executor, superclassFile) : 0);
}

int loadInstanceFieldsOfClass(ObjField** fields, ClassFile* classfile, int* offset) {
    int fieldIndex = 0;
    for (int i = 0; i < classfile->fieldPool->size; i++) {
        if (isFieldStatic(classfile->fieldPool->pool[i])) {
            continue;
        }

        UTF8* nameUtf = classfile->fieldPool->pool[i]->name;
        UTF8* descriptorUtf = classfile->fieldPool->pool[i]->descriptor;
        char* descriptor = utf82cstring(descriptorUtf);
        ObjField* field = malloc(sizeof(ObjField));

        field->offset = *offset;
        field->name = utf82cstring(nameUtf);
        fields[fieldIndex++] = field;

        *offset += getSizeFromDescriptor(descriptor);
    }
    return fieldIndex;
}

void loadInstanceFields(ObjField** fields, ClassFile* classfile, int* totalMemorySize) {
    int fieldOffset = 0;
    int fieldMemoryOffset = 0;
    while (classfile != nullptr) {
        int numFields = loadInstanceFieldsOfClass(fields + fieldOffset, classfile, &fieldMemoryOffset);
        fieldOffset += numFields;
        // We already loaded the class hierarchy, so we can assume it's loaded here.
        if (classfile->superClass == nullptr) {
            break;
        }
        classfile = classfile->superClass->classFile;
    }
    *totalMemorySize = fieldMemoryOffset;
}

int createObject(Executor* executor, GarbageCollector* gc, ClassFile* classFile) {
    if (classFile->fieldLayout == nullptr) {
        const int fieldCount = getInstanceFieldCount(executor, classFile);
        classFile->fieldLayout = malloc(sizeof(FieldLayout));
        classFile->fieldLayout->fields = malloc(sizeof(char*) * fieldCount);
        classFile->fieldLayout->fieldCount = fieldCount;
        loadInstanceFields(classFile->fieldLayout->fields, classFile, &classFile->fieldLayout->totalMemorySize);
    }
    int index = allocateNew(gc, sizeof(ObjHeader) + classFile->fieldLayout->totalMemorySize);
    ObjHeader* header = getValue(gc->memoryRegion, index);

    header->class = classFile;
    header->fieldLayout = classFile->fieldLayout;
    return index;
}

int getFieldIndex(ObjHeader* obj, char* field) {
    for (int i = 0; i < obj->fieldLayout->fieldCount; i++) {
        if (strcmp(field, obj->fieldLayout->fields[i]->name) == 0) {
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
    int offset = obj->fieldLayout->fields[fieldIndex]->offset;
    *(int32_t*) &obj->data[offset] = value;
}

int32_t getFieldValue32(ObjHeader* obj, char* field) {
    int fieldIndex = getFieldIndex(obj, field);
    if (fieldIndex == -1) {
        errorLog("Field %s is not a valid field in the class\n", field);
        return 0;
    }
    int offset = obj->fieldLayout->fields[fieldIndex]->offset;
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
