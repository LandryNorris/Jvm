#include <strings.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <fieldpoolloader.h>
#include <string.h>
#include <utf8utils.h>
#include <instructionhelper.h>
#include "interfaceparser.h"
#include "constantparser.h"
#include "primitivereader.h"
#include "classfile.h"
#include "attributehelper.h"

int initClassFile(const uint8_t * bytes, ClassFile* classPtr) {
	const uint8_t ** content = &bytes;
	classPtr->magic = readuInt32(content);
	classPtr->minor = readuInt16(content);
	classPtr->major = readuInt16(content);

	ConstantPool* cpPtr = readConstantPool(content);
	classPtr->constantPool = cpPtr;

	classPtr->accessFlags = readuInt16(content);
	classPtr->thisClass = readuInt16(content);
	classPtr->superClass = readuInt16(content);

	printf("About to call readInterfacePool\n");
	InterfacePool* ifacePtr = readInterfacePool(content);
	classPtr->interfacePool = ifacePtr;

    FieldPool* fieldPool = parseFieldPool(classPtr->constantPool, content);
    classPtr->fieldPool = fieldPool;

    MethodPool* methodPool = parseMethodPool(classPtr->constantPool, content);
    classPtr->methodPool = methodPool;

    classPtr->attributePool = parseAttributes(classPtr->constantPool, content);

	return 0;
}

const char* getTagName(ConstantPoolInfo* item) {
	switch(item->tag) {
		case CONSTANT_UTF8: return "UTF8";
		case CONSTANT_INT: return "Integer";
		case CONSTANT_FLOAT: return "Float";
		case CONSTANT_LONG: return "Long";
		case CONSTANT_DOUBLE: return "Double";
		case CONSTANT_CLASS: return "Class";
		case CONSTANT_STRING: return "String";
		case CONSTANT_FIELD_REF: return "Field Ref";
		case CONSTANT_METHOD_REF: return "Method Ref";
		case CONSTANT_INTERFACE_METHOD_REF: return "Interface Method Ref";
		case CONSTANT_NAME_AND_TYPE: return "Name and Type";
		case CONSTANT_METHOD_HANDLE: return "Method Handle";
		case CONSTANT_METHOD_TYPE: return "Method Type";
		case CONSTANT_INVOKE_DYNAMIC: return "Invoke Dynamic";
	}
	return "";
}

void getConstantString(char * const buffer, ConstantPoolInfo* item) {
	Constant* constant = item->constant;
	switch(item->tag) {
		case CONSTANT_INT:
			sprintf(buffer, "%d", constant->integer->value);
			break;
		case CONSTANT_LONG:
			sprintf(buffer, "%ldl", constant->l->value);
			break;
		case CONSTANT_FLOAT:
			sprintf(buffer, "%ff", constant->f->value);
			break;
		case CONSTANT_DOUBLE:
			sprintf(buffer, "%lfd", constant->d->value);
			break;
		case CONSTANT_UTF8:
            strcpy(buffer, utf82cstring(constant->utf8));
			break;
		case CONSTANT_NAME_AND_TYPE:
			sprintf(buffer, "#%d:#%d", constant->nameAndTypeIndex->nameIndex, constant->nameAndTypeIndex->descriptorIndex);
			break;
		case CONSTANT_CLASS:
			sprintf(buffer, "#%d", constant->class->nameIndex);
			break;
		case CONSTANT_METHOD_REF:
		case CONSTANT_FIELD_REF:
			sprintf(buffer, "#%d.#%d", constant->methodRef->classIndex, constant->methodRef->nameAndTypeIndex);
			break;
		case CONSTANT_STRING:
			sprintf(buffer, "#%d", constant->string->index);
			break;
        case CONSTANT_INVOKE_DYNAMIC:
            sprintf(buffer, "#%d:#%d", constant->invokeDynamic->bootstrapMethodAttrIndex, constant->invokeDynamic->nameAndTypeIndex);
            break;
	}
}


void printMethodPool(const ConstantPool *constantPool, const MethodPool *pool) {
    printf("\nMethod Pool\n");
    for(int i = 0; i < pool->size; i++) {
        MethodInfo* method = pool->pool[i];
        printf("\nMethod #%d\n", i+1);
        printf("%s\n", utf82cstring(constantPool->pool[method->nameIndex-1]->constant->utf8));
        printf("%s\n", utf82cstring(constantPool->pool[method->descriptorIndex-1]->constant->utf8));

        for(int j = 0; j < method->attributePool->size; j++) {
            Attribute* attribute = method->attributePool->attributes[j];
            switch(attribute->type) {
                case ATTRIBUTE_CODE: {
                    Code* code = attribute->info->code;
                    printf("Stack size: %d, Local size: %d\n", code->maxStack, code->maxLocals);
                    printProgram(code);

                    printCodeAttributes(code);
                    break;
                }
                case ATTRIBUTE_DEPRECATED: {
                    printf("\tDeprecated\n");
                }
            }
        }
    }
}

void printFieldPool(const ConstantPool *constantPool, const FieldPool *fieldPool) {
    printf("\nField Pool:\n");
    for(int i = 0; i < fieldPool->size; i++) {
        FieldPoolItem* field = fieldPool->pool[i];
        printf("\nField #%d:\n", i+1);
        printf("%s\n", utf82cstring(constantPool->pool[field->nameIndex-1]->constant->utf8));
        printf("\tAccess Flags: %x\n", field->accessFlags);
        printf("\tdescriptor: %s\n", utf82cstring(constantPool->pool[field->descriptorIndex-1]->constant->utf8));

        for(int j = 0; j < field->attributePool->size; j++) {
            Attribute* attribute = field->attributePool->attributes[j];
            switch(attribute->type) {
                case ATTRIBUTE_CONSTANT_VALUE: {
                    ConstantPoolInfo* info = constantPool->pool[attribute->info->constantValue->valueIndex-1];
                    switch(info->tag) {
                        case CONSTANT_FLOAT:
                            printf("\tConstant Value: float %f\n", info->constant->f->value);
                    }
                }
                case ATTRIBUTE_DEPRECATED: {
                    printf("\tDeprecated\n");
                    break;
                }
                case ATTRIBUTE_SYNTHETIC: {
                    printf("\tSynthetic\n");
                    break;
                }
            }
        }
    }
}

void printConstantPool(const ConstantPool *constantPool) {
    printf("\nConstant Pool:\n");
    for(int i = 1; i <= constantPool->constantPoolCount - 1; i++) {
        ConstantPoolInfo* item = constantPool->pool[i - 1];
        if(item) {
            char* constantString = calloc(60, sizeof(char));
            getConstantString(constantString, item);
            printf("%5d %-20s %s\n", i, getTagName(item), constantString);
        }
    }
}

void printClassFile(ClassFile* classFilePtr) {
	printf("Got magic value %x\n", classFilePtr->magic);
	printf("Got minor version %d\n", classFilePtr->minor);
	printf("Got major version %d\n", classFilePtr->major);
	printf("Got Constant Pool size %d\n", classFilePtr->constantPool->constantPoolCount);
    printf("Got Access Flags %d\n", classFilePtr->accessFlags);

	uint16_t thisNameIndex = classFilePtr->constantPool->pool[classFilePtr->thisClass - 1]->constant->class->nameIndex;
	UTF8* thisName = classFilePtr->constantPool->pool[thisNameIndex - 1]->constant->utf8;
	printf("This Class name is %d //%s\n", thisNameIndex, thisName->content);

	uint16_t superNameIndex = classFilePtr->constantPool->pool[classFilePtr->superClass - 1]->constant->class->nameIndex;
	UTF8* superName = classFilePtr->constantPool->pool[superNameIndex - 1]->constant->utf8;
	printf("Super Class name is //%s\n", superName->content);

	printf("Interface count: %d, Field count: %d, Method count: %d, attributes: %d\n",
           classFilePtr->interfacePool->size, classFilePtr->fieldPool->size, classFilePtr->methodPool->size,
           classFilePtr->attributePool->size);

	ConstantPool* constantPool = classFilePtr->constantPool;
    printConstantPool(constantPool);

    FieldPool* fieldPool = classFilePtr->fieldPool;
    printFieldPool(constantPool, fieldPool);

    MethodPool* pool = classFilePtr->methodPool;
    printMethodPool(constantPool, pool);
    printFileAttributes(classFilePtr->constantPool, classFilePtr->attributePool);
}
