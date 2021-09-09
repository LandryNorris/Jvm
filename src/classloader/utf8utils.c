#include "utf8utils.h"
#include<malloc.h>

char* utf82cstring(UTF8* utf8) {
    unsigned char* cstring = malloc(utf8->size+1);
    int i = 0;
    for(;i < utf8->size; i++) {
        cstring[i] = utf8->content[i];
    }
    cstring[i] = '\0'; //add null terminator
    return (char*) cstring;
}

int isEqual(UTF8* utf8, const char* cstring) {
    int size = utf8->size;

    for(int i = 0; i < size; i++) {
        char c = cstring[i];
        if(c == '\0') return 0; //there should never be a null terminator in a UTF8 string.
        if(utf8->content[i] != c) return 0; //a character doesn't match
    }
    return 1; //all characters matched.
}