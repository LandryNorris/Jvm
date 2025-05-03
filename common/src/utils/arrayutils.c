#include "utils/arrayutils.h"
#include <malloc.h>
#include <classloader/utf8utils.h>

Array* allocArray(int size, int itemSize) {
    Array* array = malloc(sizeof(Array));
    array->size = size;
    array->values = malloc(size * itemSize);
    return array;
}

CStringArray* allocCStringArray(int size) {
    CStringArray* array = malloc(sizeof(CStringArray));
    array->size = size;
    array->values = malloc(size * sizeof(char*));
    return array;
}

void freeCStringArray(CStringArray* cStringArray) {
    for(int i = 0; i < cStringArray->size; i++) {
        free(cStringArray->values[i]);
    }
    free(cStringArray->values);
    free(cStringArray);
}

int containsUtf8(CStringArray* array, UTF8* utf8) {
    for(int i = 0; i < array->size; i++) {
        if(isEqual(utf8, array->values[i])) {
            return 1;
        }
    }
    return 0;
}