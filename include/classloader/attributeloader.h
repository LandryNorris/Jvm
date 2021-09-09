#ifndef _ATTRIBUTE_LOADER_H
#define _ATTRIBUTE_LOADER_H

#include<stdint.h>
#include "constantparser.h"

//We use these values to keep track of an attribute's type. They are not defined in the JVM spec, so we can choose any
//implementation. OpenJDK seems to use a custom Symbol class, but I'll use int constants here to keep it simple,
#define ATTRIBUTE_CONSTANT_VALUE 1
#define ATTRIBUTE_CODE 2
#define ATTRIBUTE_DEPRECATED 3
#define ATTRIBUTE_SYNTHETIC 4
#define ATTRIBUTE_LINE_NUMBER_TABLE 5
#define ATTRIBUTE_LOCAL_VARIABLE_TABLE 6
#define ATTRIBUTE_LOCAL_VARIABLE_TYPE_TABLE 7
#define ATTRIBUTE_BOOTSTRAP_METHODS 8
#define ATTRIBUTE_NEST_HOST 9
#define ATTRIBUTE_NEST_MEMBERS 10
#define ATTRIBUTE_STACK_MAP_TABLE 11

#define TYPE_TOP 0
#define TYPE_INTEGER 1
#define TYPE_FLOAT 2
#define TYPE_NULL 5
#define TYPE_UNINITIALIZED_THIS 6
#define TYPE_OBJECT 7
#define TYPE_UNINITIALIZED_VARIABLE 8
#define TYPE_LONG 4
#define TYPE_DOUBLE 3

typedef struct {
	uint16_t valueIndex;
} ConstantValue;

typedef struct {
	uint16_t signatureIndex;
} Signature;

//ElementValues require a lot of work. This may work better in a real object-oriented language.
typedef struct ElementValue ElementValue;

typedef struct {
	uint16_t constValueIndex;
} ConstValue;

typedef struct {
    uint16_t typeNameIndex;
    uint16_t constNameIndex;
} EnumConstValue;

typedef struct {
    uint16_t classInfoIndex;
} ClassInfo;

typedef struct {
    uint16_t elementNameIndex;
    ElementValue* value;
} ElementValuePair;

typedef struct {
    uint16_t typeIndex;
    uint16_t numElementValuePairs;
    ElementValuePair* elementValuePairs;
} Annotation;

typedef struct {
    uint16_t size;
    ElementValue* values;
} ArrayValue;

struct ElementValue {
    uint16_t tag;

    union {
        ConstValue* constValue;
        EnumConstValue* enumValue;
        ClassInfo* classInfo;
        Annotation* annotation;
        ArrayValue* arrayValue;
    } value;
};

typedef struct {
	uint16_t count;
    Annotation* annotation;
} RuntimeAnnotations;

typedef struct {
	uint32_t length;
	uint8_t* byteCode;
} Program;

typedef struct {
	uint16_t startpc;
	uint16_t endpc;
	uint16_t handlerpc;
	uint16_t catchType;
} ExceptionAttributes;

typedef struct {
	uint16_t length;
	ExceptionAttributes** exception;
} ExceptionTable;

typedef struct {
    uint16_t startPC;
    uint16_t lineNumber;
} LineNumberElement;

typedef struct {
    uint16_t size;
    LineNumberElement** entries;
} LineNumberTable;

typedef struct {
    uint16_t startPC;
    uint16_t length;
    uint16_t nameIndex;
    uint16_t descriptorIndex;
    uint16_t index;
} LocalVariableElement;

typedef struct {
    uint16_t size;
    LocalVariableElement** entries;
} LocalVariableTable;

typedef struct {
    uint16_t startPC; //the variable has a value in the byteCode array in the range [startPC, startPC+length).
    uint16_t length;
    uint16_t nameIndex; //Constant Pool entry giving the name of the variable.
    uint16_t signatureIndex; //Constant Pool entry giving the signature of the variable.
    uint16_t index; //index into the local variable array of the current frame.
} LocalVariableTypeElement;

typedef struct {
    uint16_t size;
    LocalVariableTypeElement** entries;
} LocalVariableTypeTable;

typedef struct {
    uint8_t tag;
    uint16_t index; //only used for Object and Uninitialized.
} VerificationType;

typedef struct {
    //values of frameType in [0,63] mean variables same as last frame and operation stack is empty.
    //values of frameType in [64,127] mean there is one item in the stack. Value of offsetDelta is frameType - 64.
    //value of frameType = 247 means extended one item in stack, where offsetDelta is explicitly given.
    //values of frameType in [248,250] mean that the local variables are the same as the last frame, but the operand
    //stack is empty. The latest k = 251-frameType local variables are absent.
    //value of frameType = 251 means local variables are the same as the last frame and op stack is empty, but offset delta is given.
    //values of frameType in [252-254] mean that the local variables are same as last frame, but k = frameType - 251 new local variables are added.
    //value of frameType = 255 means a 'full stack', which uses all members.
    uint8_t frameType;
    uint16_t offsetDelta;
    uint16_t stackSize;
    VerificationType** stack;
    uint16_t numLocals;
    VerificationType** localVariables;
} StackMapFrame;

typedef struct {
    uint16_t size;
    StackMapFrame** entries;
} StackMapTable;

typedef struct {
    uint16_t type;
    uint16_t nameIndex;
    uint32_t attributeLength;
    union {
        LineNumberTable* lineNumberTable;
        LocalVariableTable* localVariableTable;
        LocalVariableTypeTable* localVariableTypeTable;
        StackMapTable* stackMapTable;
    } tables;
} CodeAttributes;

typedef struct {
    uint16_t maxStack;
    uint16_t maxLocals;
    Program* program;
    ExceptionTable* exceptionTable;
    CodeAttributes** attributeInfo;
} Code;

typedef union {
	ConstantValue* constantValue;
	Code* code;
    RuntimeAnnotations* runtimeVisibleAnnotations;
    Signature* signature;
} AttributeInfo;

typedef struct {
    int type;
    uint16_t nameIndex;
    uint32_t attributeLength;
    AttributeInfo* info;
} Attribute;

typedef struct {
    uint16_t size;
    Attribute** attributes;
} AttributePool;

AttributePool* parseAttributes(ConstantPool*, const uint8_t**);

#endif
