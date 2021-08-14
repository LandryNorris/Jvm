#include <strings.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include "interfaceparser.h"
#include "constantparser.h"
#include "classfile.h"

int initClassFile(const uint8_t const* bytes, ClassFile* classPtr) {
	uint8_t** content = &bytes;
	classPtr->magic = readuInt32(content);
	classPtr->minor = readuInt16(content);
	classPtr->major = readuInt16(content);

	ConstantPool* cpPtr = readConstantPool(content);
	classPtr->constantPool = cpPtr;

	classPtr->accessFlags = readuInt16(content);
	classPtr->thisClass = readuInt16(content);
	classPtr->superClass = readuInt16(content);

	InterfacePool* ifacePtr = readInterfacePool(content);
	classPtr->interfacePool = ifacePtr;

	return 0;
}

void printClassFile(ClassFile* classFilePtr) {
	printf("Got magic value %x\n", classFilePtr->magic);
	printf("Got minor version %d\n", classFilePtr->minor);
	printf("Got major version %d\n", classFilePtr->major);
	printf("Got Constant Pool size %d\n", classFilePtr->constantPool->constantPoolCount);
	printf("Class name is %d, %s\n", classFilePtr->thisClass, classFilePtr->constantPool->constantPool[classFilePtr->thisClass]);
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
