#include "constantparser.h"
#include "utf8utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "constantpoolhelper.h"

char* parseUTF8(UTF8* utf8) {
    return utf82cstring(utf8);
}

char* parseUTF8ByIndex(int index, const ConstantPool* constantPool) {
    return parseUTF8(constantPool->pool[index-1]->constant->utf8);
}

char* parseString(String* string, const ConstantPool* constantPool) {
    int index = string->index;
    return parseUTF8ByIndex(index, constantPool);
}

char* parseClass(Class* class, const ConstantPool* constantPool) {
    int nameIndex = class->nameIndex;
    return parseUTF8ByIndex(nameIndex, constantPool);
}

UTF8* parseClassToUTF8(Class* class, const ConstantPool* constantPool) {
    return constantPool->pool[class->nameIndex-1]->constant->utf8;
}

UTF8* parseClassToUTF8ByIndex(int index, const ConstantPool* constantPool) {
    Class* class = constantPool->pool[index-1]->constant->class;
    return parseClassToUTF8(class, constantPool);
}

char* parseClassByIndex(int index, const ConstantPool* constantPool) {
    return parseClass(constantPool->pool[index-1]->constant->class, constantPool);
}

char* parseNameAndType(NameAndTypeIndex* nameAndTypeIndex, const ConstantPool* constantPool) {
    char* nameString = parseUTF8ByIndex(nameAndTypeIndex->nameIndex, constantPool);
    char* typeString = parseUTF8ByIndex(nameAndTypeIndex->descriptorIndex, constantPool);
    char* result = malloc(strlen(nameString) + 1 + strlen(typeString) + 1);

    strcpy(result, nameString);
    strcat(result, ":");
    strcat(result, typeString);

    free(nameString);
    free(typeString);
    return result;
}

char* parseNameAndTypeByIndex(int index, const ConstantPool* constantPool) {
    return parseNameAndType(constantPool->pool[index-1]->constant->nameAndTypeIndex, constantPool);
}

char* parseMethodRef(MethodRef* methodRef, const ConstantPool* constantPool) {
    char* classString = parseClassByIndex(methodRef->classIndex, constantPool);
    char* nameString = parseNameAndTypeByIndex(methodRef->nameAndTypeIndex, constantPool);

    char* result = malloc(strlen(classString) + 1 + strlen(nameString) + 1);

    strcpy(result, classString);
    strcat(result, ".");
    strcat(result, nameString);

    free(classString);
    free(nameString);
    return result;
}

char* parseMethodRefByIndex(int index, const ConstantPool* constantPool) {
    return parseMethodRef(constantPool->pool[index-1]->constant->methodRef, constantPool);
}

char* parseMethodHandle(MethodHandle* methodHandle, const ConstantPool* constantPool) {
    char* methodDescription = parseMethodRefByIndex(methodHandle->referenceIndex, constantPool);
    char* result = malloc(3+strlen(methodDescription) + 1);

    snprintf(result, strlen(methodDescription), "%2d:%s", methodHandle->referenceKind, methodDescription);
    return result;
}

char* parseInteger(Integer* integer) {
    if(integer == NULL) return NULL;
    int len = integer->value==0 ? 1 : (int)(log10(abs(integer->value)))+1;
    if (integer->value<0) len++; // room for negative sign '-'

    char* buf = calloc(sizeof(char), len+1); // +1 for null
    snprintf(buf, len+1, "%d", integer->value);
    return buf;
}

char* parseFloat(Float* f) {
    if(f == NULL) return NULL;
    int len = 7;
    if (f->value<0) len++; // room for negative sign '-'

    char* buf = calloc(sizeof(char), len+1); // +1 for null
    snprintf(buf, len+1, "%f", f->value);
    return buf;
}