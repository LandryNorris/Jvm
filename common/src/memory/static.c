#include "memory/static.h"

#include <stdlib.h>
#include <string.h>

#include "classloader/utf8utils.h"
#include "memory/objheader.h"

void initializeStaticFields(ClassFile* classFile) {
    const int fieldCount = classFile->fieldPool->size;

    StaticFields* staticFields = malloc(sizeof(StaticFields));
    staticFields->fields = malloc(sizeof(StaticField) * fieldCount);

    // TODO(Landry): Account for instance fields
    staticFields->numFields = fieldCount;

    for(int i = 0; i < fieldCount; i++) {
        // we only care about instance fields
        if (!isFieldStatic(classFile->fieldPool->pool[i])) {
            continue;
        }

        const int nameIndex = classFile->fieldPool->pool[i]->nameIndex;
        const int descriptorIndex = classFile->fieldPool->pool[i]->descriptorIndex;
        UTF8* nameUtf = classFile->constantPool->pool[nameIndex-1]->constant->utf8;
        UTF8* descriptorUtf = classFile->constantPool->pool[descriptorIndex-1]->constant->utf8;
        char* descriptor = utf82cstring(descriptorUtf);
        StaticField field;

        field.value = 0;
        field.name = utf82cstring(nameUtf);
        staticFields->fields[i] = field;
    }

    classFile->staticFields = staticFields;
}

StaticField* getStaticField(ClassFile* classFile, const char* field) {
    if (classFile->staticFields == nullptr) {
        initializeStaticFields(classFile);
    }

    for (int i = 0; i < classFile->staticFields->numFields; i++) {
        StaticField* staticField = &classFile->staticFields->fields[i];
        if (strcmp(staticField->name, field) == 0) {
            return staticField;
        }
    }
    return nullptr;
}

int32_t getInt32StaticField(ClassFile* classFile, const char* field) {
    const StaticField* staticField = getStaticField(classFile, field);

    return (int32_t)staticField->value;
}

int32_t setInt32StaticField(ClassFile* classFile, const char* field, int32_t value) {
    StaticField* staticField = getStaticField(classFile, field);
    staticField->value = value;
}
