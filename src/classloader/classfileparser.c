#include <strings.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include "interfaceparser.h"
#include "constantparser.h"
#include "primitivereader.h"
#include "classfile.h"

int initClassFile(const uint8_t const* bytes, ClassFile* classPtr) {
	uint8_t const** content = &bytes;
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

void getConstantString(char * buffer, ConstantPoolInfo* item) {
	Constant* constant = item->constant;
	switch(item->tag) {
		case CONSTANT_INT:
			sprintf(buffer, "%d", constant->integer.value);
			break;
		case CONSTANT_LONG:
			sprintf(buffer, "%ldl", constant->l.value);
			break;
		case CONSTANT_FLOAT:
			sprintf(buffer, "%ff", constant->f.value);
			break;
		case CONSTANT_DOUBLE:
			sprintf(buffer, "%lfd", constant->d.value);
			break;
		case CONSTANT_UTF8:
			for(int i = 0; i < constant->utf8.size; i++) {
				buffer[i] = constant->utf8.content[i];
			}
			break;
		case CONSTANT_NAME_AND_TYPE:
			sprintf(buffer, "#%d:#%d", constant->nameAndTypeIndex.nameIndex, constant->nameAndTypeIndex.descriptorIndex);
			break;
		case CONSTANT_CLASS:
			sprintf(buffer, "#%d", constant->class.nameIndex);
			break;
		case CONSTANT_METHOD_REF:
			sprintf(buffer, "#%d.#%d", constant->methodRef.classIndex, constant->methodRef.nameAndTypeIndex);
			break;
		case CONSTANT_FIELD_REF:
			sprintf(buffer, "#%d.#%d", constant->methodRef.classIndex, constant->methodRef.nameAndTypeIndex);
			break;
	}
}

void printClassFile(ClassFile* classFilePtr) {
	printf("Got magic value %x\n", classFilePtr->magic);
	printf("Got minor version %d\n", classFilePtr->minor);
	printf("Got major version %d\n", classFilePtr->major);
	printf("Got Constant Pool size %d\n", classFilePtr->constantPool->constantPoolCount);

	uint16_t thisNameIndex = classFilePtr->constantPool->constantPool[classFilePtr->thisClass-1]->constant->class.nameIndex;
	UTF8 thisName = classFilePtr->constantPool->constantPool[thisNameIndex-1]->constant->utf8;
	printf("This Class name is %d %s\n", thisNameIndex, thisName.content);

	uint16_t superNameIndex = classFilePtr->constantPool->constantPool[classFilePtr->superClass-1]->constant->class.nameIndex;
	UTF8 superName = classFilePtr->constantPool->constantPool[superNameIndex-1]->constant->utf8;
	printf("Super Class name is %s\n", superName.content);

	printf("Interface count: %d, Field count: %d, Method count: %d, attributes: %d\n\n", classFilePtr->interfacePool->size, 0, 0, 0);
	printf("Constant Pool:\n");

	ConstantPool* constantPool = classFilePtr->constantPool;
	for(int i = 1; i <= constantPool->constantPoolCount-1; i++) {
		ConstantPoolInfo* item = constantPool->constantPool[i-1];
		if(item) {
			char* constantString = calloc(60, sizeof(char));
			getConstantString(constantString, item);
			printf("%5d %-20s %s\n", i, getTagName(item), constantString);
		}
	}
}

int main(int argc, char* argv[]) {
	if(argc < 2) {
		printf("no class file given.\n");
		return EINVAL;
	}
	const char* classFilePath = argv[1];
	printf("Trying to open file at %s\n", classFilePath);
	FILE* file = fopen(classFilePath, "r");

	if(file == NULL) {
		printf("No such file\n");
		return ENOENT;
	}

	//Read length
	fseek(file, 0L, SEEK_END);
	int lengthBytes = ftell(file);
	fseek(file, 0L, SEEK_SET);

	uint8_t* content = malloc(lengthBytes+1);

	fread(content, lengthBytes, 1, file);

	ClassFile* classFilePtr = malloc(sizeof(ClassFile));
	initClassFile(content, classFilePtr);

	printClassFile(classFilePtr);

	fclose(file);
	free(content);
	free(classFilePtr);

	return 0;
}
