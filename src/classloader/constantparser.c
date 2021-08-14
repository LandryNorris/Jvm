#include "constantparser.h"
#include <stdlib.h>
#include <stdio.h>
#include "primitivereader.h"

Constant* readUtf8(const uint8_t** bytes) {
	UTF8* utf8 = malloc(sizeof(UTF8));
	utf8->size = readuInt16(bytes);

	utf8->content = malloc((utf8->size+1) * sizeof(uint8_t)); //Include null terminator.
	for(int i = 0; i < utf8->size; i++) {
		utf8->content[i] = readuInt8(bytes);
	}
	Constant* result = malloc(sizeof(Constant));
	result->utf8 = *utf8;
	return result;
}

Constant* readMethodRef(const uint8_t** bytes) {
	MethodRef* methodRef = malloc(sizeof(MethodRef));
	methodRef->classIndex = readuInt16(bytes);
	methodRef->nameAndTypeIndex = readuInt16(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->methodRef = *methodRef;
	return result;
}

Constant* readClass(const uint8_t** bytes) {
	Class* class = malloc(sizeof(Class));
	class->nameIndex = readuInt16(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->class = *class;
	return result;
}

Constant* readNameAndType(const uint8_t** bytes) {
	NameAndTypeIndex* nameAndTypeIndex = malloc(sizeof(NameAndTypeIndex));
	nameAndTypeIndex->nameIndex = readuInt16(bytes);
	nameAndTypeIndex->descriptorIndex = readuInt16(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->nameAndTypeIndex = *nameAndTypeIndex;
	return result;
}

Constant* readInt(const uint8_t** bytes) {
	Integer* i = malloc(sizeof(Integer));
	i->value = readuInt32(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->integer = *i;
	return result;
}

Constant* readConstantFloat(const uint8_t** bytes) {
	Float* f = malloc(sizeof(Float));
	f->value = readFloat(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->f = *f;
	return result;
}

Constant* readLong(const uint8_t** bytes) {
	Long* l = malloc(sizeof(Long));
	l->value = readuInt64(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->l = *l;
	return result;
}

Constant* readConstantDouble(const uint8_t** bytes) {
	Double* d = malloc(sizeof(Double));
	d->value = readDouble(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->d = *d;
	return result;
}

Constant* readString(const uint8_t** bytes) {
	String* i = malloc(sizeof(String));
	i->index = readuInt16(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->string = *i;
	return result;
}

Constant* readMethodHandle(const uint8_t** bytes) {
	MethodHandle* ref = malloc(sizeof(MethodHandle));
	ref->referenceKind = readuInt8(bytes);
	ref->referenceType = readuInt16(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->methodHandle = *ref;
	return result;
}

Constant* readMethodType(const uint8_t** bytes) {
	MethodType* type = malloc(sizeof(MethodType));
	type->descriptorIndex = readuInt16(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->methodType = *type;
	return result;
}

ConstantPoolInfo* readPoolInfo(const uint8_t** bytes) {
	uint8_t tag = readuInt8(bytes);
	Constant* constantPtr = malloc(sizeof(Constant));

	switch(tag) {
		case CONSTANT_UTF8:
			constantPtr = readUtf8(bytes);
			break;
		case CONSTANT_INT:
			constantPtr = readInt(bytes);
			break;
		case CONSTANT_FLOAT:
			constantPtr = readConstantFloat(bytes);
			break;
		case CONSTANT_LONG:
			constantPtr = readLong(bytes);
			break;
		case CONSTANT_DOUBLE:
			constantPtr = readConstantDouble(bytes);
			break;
		case CONSTANT_CLASS:
			constantPtr = readClass(bytes);
			break;
		case CONSTANT_STRING:
			constantPtr = readString(bytes);
			break;
		//These all have the same format. I'll save some code and parse all the same. It's all going into a byte array anyways.
		case CONSTANT_FIELD_REF:
		case CONSTANT_METHOD_REF:
		case CONSTANT_INTERFACE_METHOD_REF:
			constantPtr = readMethodRef(bytes);
			break;
		case CONSTANT_NAME_AND_TYPE:
			constantPtr = readNameAndType(bytes);
			break;
		case CONSTANT_METHOD_HANDLE:
			constantPtr = readMethodHandle(bytes);
			break;
		case CONSTANT_METHOD_TYPE:
			constantPtr = readMethodType(bytes);
			break;
		//Add support for invokeDynamic later.
	}
	ConstantPoolInfo* result = malloc(sizeof(ConstantPoolInfo));
	result->tag = tag;
	result->constant = constantPtr;
	return result;
}

ConstantPool* readConstantPool(const uint8_t** bytes) {
	uint16_t size = readuInt16(bytes);

	ConstantPool* result = malloc(sizeof(ConstantPool)+size*sizeof(ConstantPoolInfo*));
	result->constantPoolCount = size;

	for(int i = 0; i < size-1; i++) {
		ConstantPoolInfo* cpInfoPtr = readPoolInfo(bytes);
		result->constantPool[i] = cpInfoPtr;
	}

	return result;
}
