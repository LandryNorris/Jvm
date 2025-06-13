#include "classloader/constantparser.h"
#include <stdlib.h>
#include "classloader/primitivereader.h"

Constant* readUtf8(const uint8_t** bytes) {
	UTF8* utf8 = malloc(sizeof(UTF8));
	utf8->size = readuInt16(bytes);

	utf8->content = malloc((utf8->size+1) * sizeof(uint8_t)); //Include null terminator.
	for(int i = 0; i < utf8->size; i++) {
		utf8->content[i] = readuInt8(bytes);
	}
	Constant* result = malloc(sizeof(Constant));
	result->utf8 = utf8;
	return result;
}

Constant* readMethodRef(const uint8_t** bytes) {
	MethodRef* methodRef = malloc(sizeof(MethodRef));
	methodRef->classIndex = readuInt16(bytes);
	methodRef->nameAndTypeIndex = readuInt16(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->methodRef = methodRef;
	return result;
}

Constant* readClass(const uint8_t** bytes) {
	Class* class = malloc(sizeof(Class));
	uint16_t nameIndex = readuInt16(bytes);
	class->nameIndex = nameIndex;
    class->classFile = NULL;
	Constant* result = malloc(sizeof(Constant));
	result->class = class;
	return result;
}

Constant* readNameAndType(const uint8_t** bytes) {
	NameAndType* nameAndTypeIndex = malloc(sizeof(NameAndType));
	nameAndTypeIndex->nameIndex = readuInt16(bytes);
	nameAndTypeIndex->descriptorIndex = readuInt16(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->nameAndType = nameAndTypeIndex;
	return result;
}

Constant* readInt(const uint8_t** bytes) {
	Integer* i = malloc(sizeof(Integer));
	i->value = readInt32(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->integer = i;
	return result;
}

Constant* readConstantFloat(const uint8_t** bytes) {
	Float* f = malloc(sizeof(Float));
	f->value = readFloat(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->f = f;
	return result;
}

Constant* readLong(const uint8_t** bytes) {
	Long* l = malloc(sizeof(Long));
	l->value = readuInt64(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->l = l;
	return result;
}

Constant* readConstantDouble(const uint8_t** bytes) {
	Double* d = malloc(sizeof(Double));
	d->value = readDouble(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->d = d;
	return result;
}

Constant* readString(const uint8_t** bytes) {
	String* i = malloc(sizeof(String));
	i->index = readuInt16(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->string = i;
	return result;
}

Constant* readMethodHandle(const uint8_t** bytes) {
	MethodHandle* ref = malloc(sizeof(MethodHandle));
	ref->referenceKind = readuInt8(bytes);
	ref->referenceIndex = readuInt16(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->methodHandle = ref;
	return result;
}

Constant* readMethodType(const uint8_t** bytes) {
	MethodType* type = malloc(sizeof(MethodType));
	type->descriptorIndex = readuInt16(bytes);
	Constant* result = malloc(sizeof(Constant));
	result->methodType = type;
	return result;
}

Constant* readInvokeDynamic(const uint8_t** bytes) {
    InvokeDynamic* invokeDynamic = malloc(sizeof(InvokeDynamic));
    invokeDynamic->bootstrapMethodAttrIndex = readuInt16(bytes);
    invokeDynamic->nameAndTypeIndex = readuInt16(bytes);
    Constant* result = malloc(sizeof(Constant));
    result->invokeDynamic = invokeDynamic;
    return result;
}

ConstantPoolInfo* readPoolInfo(const uint8_t** bytes) {
	uint8_t tag = readuInt8(bytes);
	Constant* constantPtr = NULL;

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
		//These all have the same format. I'll save some byteCode and parse all the same. It's all going into a byte array anyways.
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
        case CONSTANT_INVOKE_DYNAMIC:
            constantPtr = readInvokeDynamic(bytes);
            break;
	}
	ConstantPoolInfo* result = malloc(sizeof(ConstantPoolInfo));
	result->tag = tag;
	result->constant = constantPtr;
	return result;
}

ConstantPool* readConstantPool(const uint8_t** bytes) {
	uint16_t size = readuInt16(bytes)-1; //The JVM spec says the given size is actual size+1

	ConstantPool* result = malloc(sizeof(ConstantPool));
	result->size = size;
    result->pool = malloc(size * sizeof(ConstantPoolInfo*));

	for(int i = 0; i < size; i++) {
		ConstantPoolInfo* cpInfoPtr = readPoolInfo(bytes);
		result->pool[i] = cpInfoPtr;
		if(cpInfoPtr->tag == CONSTANT_DOUBLE || cpInfoPtr->tag == CONSTANT_LONG) {
			result->pool[++i] = NULL; //we want to skip a spot for doubles and longs.
		}
	}

	// All items in the pool are now valid. Fill out shortcuts
	for (int i = 0; i < size; i++) {
		ConstantPoolInfo* cpInfoPtr = result->pool[i];
		switch (cpInfoPtr->tag) {
			case CONSTANT_CLASS: {
				cpInfoPtr->constant->class->name = result->pool[cpInfoPtr->constant->class->nameIndex-1]->constant->utf8;
				break;
			}
			case CONSTANT_NAME_AND_TYPE: {
				NameAndType* nameAndTypePtr = cpInfoPtr->constant->nameAndType;
				nameAndTypePtr->name = result->pool[nameAndTypePtr->nameIndex-1]->constant->utf8;
				nameAndTypePtr->descriptor = result->pool[nameAndTypePtr->descriptorIndex-1]->constant->utf8;
				break;
			}
			case CONSTANT_FIELD_REF:
			case CONSTANT_INTERFACE_METHOD_REF:
			case CONSTANT_METHOD_REF: {
				cpInfoPtr->constant->methodRef->nameAndType = result->pool[cpInfoPtr->constant->methodRef->nameAndTypeIndex-1]->constant->nameAndType;
				cpInfoPtr->constant->methodRef->class = result->pool[cpInfoPtr->constant->methodRef->classIndex-1]->constant->class;
				break;
			}
			case CONSTANT_STRING: {
				cpInfoPtr->constant->string->string = result->pool[cpInfoPtr->constant->string->index-1]->constant->utf8;
				break;
			}
			default: {}
		}
	}

	return result;
}
