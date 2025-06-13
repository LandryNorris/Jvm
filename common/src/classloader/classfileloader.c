#include "classloader/classfileloader.h"
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <malloc.h>
#include <classloader/classfile.h>
#include <dirent.h>
#include <string.h>
#include <classloader/utf8utils.h>
#include <utils/constantpoolhelper.h>

ClassFile* loadClassFile(const char* classFilePath) {
    FILE* file = fopen(classFilePath, "r");

    if(file == NULL) {
        printf("No such file %s\n", classFilePath);
        return NULL;
    }

    //Read length
    fseek(file, 0L, SEEK_END);
    long lengthBytes = ftell(file);
    fseek(file, 0L, SEEK_SET);

    uint8_t* content = malloc(lengthBytes+1);

    fread(content, lengthBytes, 1, file);

    ClassFile* classFilePtr = malloc(sizeof(ClassFile));
    initClassFile(content, classFilePtr);

    fclose(file);
    free(content);

    return classFilePtr;
}

CStringArray* getDependencyClasses(ClassFile* classFile) {
    ConstantPool* constantPool = classFile->constantPool;

    //number of classes referenced in the constant pool of the given classFile
    int numClasses = 0;
    for(int i = 0; i < constantPool->size; i++) {
        if(!constantPool->pool[i]) continue;
        int tag = constantPool->pool[i]->tag;
        if(tag == CONSTANT_CLASS) numClasses++;
    }
    numClasses--; // thisClass shouldn't count

    CStringArray* result = allocCStringArray(numClasses);
    int numResultsWritten = 0;
    for(int i = 0; i < constantPool->size; i++) {
        if(!constantPool->pool[i]) continue; //spot is empty
        if(constantPool->pool[i]->tag == CONSTANT_CLASS) {
            //We need to add 1 to the index because class files are 1-indexed
            UTF8* nameUtf8 = parseClassToUTF8ByIndex(i+1, constantPool);

            if (nameUtf8 == classFile->name) continue; // skip this class

            CStringArray* subList = malloc(sizeof(CStringArray));
            subList->values = result->values;
            subList->size = numResultsWritten;
            if(containsUtf8(subList, nameUtf8)) continue;
            char* name = utf82cstring(nameUtf8);
            result->values[numResultsWritten++] = name;
        }
    }
    return result;
}

int isClassFile(char* filename) {
    char* ext = strrchr(filename, '.');
    if(ext) {
        return strcmp(ext, ".class") == 0;
    }
    return 0;
}

int findClassFile(const char* classFileDirectory, const char* name) {
    char* tryPath = malloc(1000);
    strcpy(tryPath, classFileDirectory);
    strcat(tryPath, "/");
    strcat(tryPath, name);
    strcat(tryPath, ".class");
    FILE* classFile = fopen(tryPath, "r");

    if(classFile) {
        printf("Found ClassFile at %s\n", tryPath);
        return 0;
    }
    return ENOENT;
}

int loadClassFilesRecursive(const char* classFileDirectory) {
    char inner[1000];
    DIR* directory = opendir(classFileDirectory);
    if(!directory) return ENOTDIR;
    struct dirent* dir;
    while((dir = readdir(directory)) != NULL) {
        if(dir->d_type == 4 && strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0) { //Folder
            strcpy(inner, classFileDirectory);
            strcat(inner, "/");
            strcat(inner, dir->d_name);
            loadClassFilesRecursive(inner);
        } else if(dir->d_type == 8) { //File
            if(isClassFile(dir->d_name)) {
                printf("Found Class File %s\n", dir->d_name);
            }
        }
    }
    closedir(directory);
    return 0;
}

void freeClassFile(ClassFile* classFile) {
    freeConstantPool(classFile->constantPool);
    freeMethodPool(classFile->methodPool);
    freeAttributePool(classFile->attributePool);
    freeInterfacePool(classFile->interfacePool);
    freeFieldPool(classFile->fieldPool);
    free(classFile);
}

void freeInterfacePool(InterfacePool* interfacePool) {
    free(interfacePool);
}

void freeFieldPool(FieldPool *fieldPool) {
    for(int i = 0; i < fieldPool->size; i++) {
        FieldPoolItem* item = fieldPool->pool[i];
        freeAttributePool(item->attributePool);
        free(item);
    }
    free(fieldPool->pool);
    free(fieldPool);
}

void freeMethodPool(MethodPool* methodPool) {
    for(int i = 0; i < methodPool->size; i++) {
        MethodInfo* methodInfo = methodPool->pool[i];
        freeAttributePool(methodInfo->attributePool);
        free(methodInfo);
    }
    free(methodPool->pool);
    free(methodPool);
}

void freeAttributePool(AttributePool *attributePool) {
    for(int j = 0; j < attributePool->size; j++) {
        Attribute* attribute = attributePool->attributes[j];
        switch(attribute->type) {
            case ATTRIBUTE_CODE: {
                Code* code = attribute->info->code;
                for(int k = 0; k < code->numAttributes; k++) {
                    CodeAttributes* codeAttributes = code->attributeInfo[k];

                    switch(codeAttributes->type) {
                        case ATTRIBUTE_STACK_MAP_TABLE: {
                            freeStackMapTable(codeAttributes->tables.stackMapTable);
                            break;
                        }
                        case ATTRIBUTE_LINE_NUMBER_TABLE: {
                            free(codeAttributes->tables.lineNumberTable->entries);
                            break;
                        }
                        case ATTRIBUTE_LOCAL_VARIABLE_TABLE: {
                            free(codeAttributes->tables.localVariableTable->entries);
                            break;
                        }
                        case ATTRIBUTE_LOCAL_VARIABLE_TYPE_TABLE: {
                            free(codeAttributes->tables.localVariableTypeTable->entries);
                        }
                    }
                    free(codeAttributes->tables.localVariableTypeTable); //pointers in a union. We free one.
                    free(codeAttributes);
                }
                free(code->attributeInfo);
                free(code->exceptionTable);
                free(code->program->byteCode);
                free(code->program);
                free(code);
                break;
            }
            case ATTRIBUTE_BOOTSTRAP_METHODS: {
                for(int k = 0; k < attribute->info->bootstrapMethods->count; k++) {
                    free(attribute->info->bootstrapMethods->methods[k].arguments);
                }
                free(attribute->info->bootstrapMethods->methods);
                free(attribute->info->bootstrapMethods);
                break;
            }
            case ATTRIBUTE_CONSTANT_VALUE: {
                free(attribute->info->constantValue);
                break;
            }
            case ATTRIBUTE_NEST_HOST: {
                free(attribute->info->nestHost);
                break;
            }
            case ATTRIBUTE_NEST_MEMBERS: {
                free(attribute->info->nestMembers->classes);
                free(attribute->info->nestMembers);
                break;
            }
        }
        free(attribute->info);
        free(attribute);
    }
    free(attributePool->attributes);
    free(attributePool);
}

void freeStackMapTable(StackMapTable *stackMapTable) {
    for(int i = 0; i < stackMapTable->size; i++) {
        StackMapFrame* frame = &stackMapTable->entries[i];
        for(int j = 0; j < frame->stackSize; j++) {
            free(frame->stack[j]);
        }
        for(int j = 0; j < frame->numLocals; j++) {
            free(frame->localVariables[j]);
        }
        free(frame->stack);
        free(frame->localVariables);
    }
    free(stackMapTable->entries);
}

void freeConstantPool(ConstantPool* constantPool) {
    for(int i = 0; i < constantPool->size; i++) {
        ConstantPoolInfo* info = constantPool->pool[i];
        if(info == NULL) continue;
        if(info->tag == CONSTANT_UTF8) {
            free(info->constant->utf8->content); //UTF8 can have malloced content inside.
        }
        free(info->constant->integer); //Union of pointers. Maybe we only have to free one? None of these hold objects themselves (besides UTF8), just primitives.
        free(info->constant);
        free(info);
    }
    free(constantPool->pool);
    free(constantPool);
}
