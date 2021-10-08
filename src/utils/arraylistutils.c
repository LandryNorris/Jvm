#include <malloc.h>
#include "arraylistutils.h"

CStringArrayList* allocCStringArrayList(int size) {
    CStringArrayList* result = malloc(sizeof(CStringArrayList));
    result->size = size;
    result->currentIndex = 0;
    result->values = malloc(size*sizeof(char*));
    return result;
}

CStringArrayList* copyArrayList(CStringArrayList *list, int size) {
    CStringArrayList* result = allocCStringArrayList(size);
    for(int i = 0; i < list->size; i++) {
        result->values[i] = list->values[i];
    }
    return result;
}

void add(CStringArrayList* arrayList, char* cstring) {
    if(arrayList->currentIndex >= arrayList->size) {
        CStringArrayList* newList = copyArrayList(arrayList, (int)(arrayList->size*1.5));
        free(arrayList); //free the old list
        arrayList = newList; //use the list we just allocated.
    }
    arrayList->values[arrayList->currentIndex++] = cstring;
}