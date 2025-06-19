#include "classloader/utf8utils.h"

#include <malloc.h>
#include <string.h>

#include "classloader/classloader.h"
#include "memory/objheader.h"
#include "memory/primitive_array.h"

int utf82string(GarbageCollector* gc, const ClassLoader* loader, const UTF8* utf8) {
    ClassFile* stringClass = getClassFile(loader, "java/lang/String", nullptr);
    int index = createObject(gc, stringClass);
    ObjHeader* header = getValue(gc->memoryRegion, index);

    // Create value array
    const int arrayOffset = createPrimitiveArray(gc, T_BYTE, utf8->size);
    PrimitiveArray* arrayHeader = getValue(gc->memoryRegion, arrayOffset);

    uint8_t* textLocation = arrayHeader->memory;

    int j = 0;
    for (; j < utf8->size; j++) {
        textLocation[j] = utf8->content[j];
    }
    textLocation[j] = '\0'; // add null terminator

    // TODO(Landry): I think openjdk hardcodes the index. Maybe that makes sense here too?
    for (int i = 0; i < header->fieldCount; i++) {
        ObjField* field = header->fields[i];

        // Look for 'value' field
        if (strcmp(field->name, "value") == 0) {
            ObjField* field = header->fields[i];

            memcpy(&header->data[field->offset], &arrayOffset, sizeof(int));
        }
    }

    return index;
}

char* utf82cstring(UTF8* utf8) {
    unsigned char* cstring = malloc(utf8->size + 1);
    int i = 0;
    for (; i < utf8->size; i++) {
        cstring[i] = utf8->content[i];
    }
    cstring[i] = '\0'; // add null terminator
    return (char*) cstring;
}

int isEqual(UTF8* utf8, const char* cstring) {
    int size = utf8->size;

    if (strlen(cstring) != size) {
        return 0; // size doesn't match
    }

    for (int i = 0; i < size; i++) {
        char c = cstring[i];
        if (c == '\0') return 0; // there should never be a null terminator in a UTF8 string.
        if (utf8->content[i] != c) return 0; // a character doesn't match
    }
    return 1; // all characters matched.
}

int isEqualUtf8(UTF8* utf1, UTF8* utf2) {
    if (utf1->size != utf2->size) return 0;

    for (int i = 0; i < utf1->size; i++) {
        if (utf1->content[i] != utf2->content[i]) return 0;
    }
    return 1;
}

void initUtf8(UTF8* utf8, const char* cstring) {
    utf8->size = strlen(cstring);
    utf8->content = (uint8_t*) cstring;
}
