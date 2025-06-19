#ifndef JVM_PROJECT_CONSTANT_PARSER_H
#define JVM_PROJECT_CONSTANT_PARSER_H

#include <stdint.h>

#define CONSTANT_UTF8 1
#define CONSTANT_INT 3
#define CONSTANT_FLOAT 4
#define CONSTANT_LONG 5
#define CONSTANT_DOUBLE 6
#define CONSTANT_CLASS 7
#define CONSTANT_STRING 8
#define CONSTANT_FIELD_REF 9
#define CONSTANT_METHOD_REF 10
#define CONSTANT_INTERFACE_METHOD_REF 11
#define CONSTANT_NAME_AND_TYPE 12
#define CONSTANT_METHOD_HANDLE 15
#define CONSTANT_METHOD_TYPE 16
#define CONSTANT_INVOKE_DYNAMIC 18

typedef struct ClassFile ClassFile;

typedef struct {
    uint16_t size;
    uint8_t* content;
} UTF8;

typedef struct {
    uint16_t nameIndex;
    UTF8* name;
    ClassFile* classFile;
} Class;

typedef struct {
    uint16_t nameIndex;
    uint16_t descriptorIndex;
    UTF8* name;
    UTF8* descriptor;
} NameAndType;

typedef struct {
    uint16_t classIndex;
    uint16_t nameAndTypeIndex;
    NameAndType* nameAndType;
    Class* class;
} MethodRef;

typedef struct {
    int32_t value;
} Integer;

typedef struct {
    float value;
} Float;

typedef struct {
    uint64_t value;
} Long;

typedef struct {
    double value;
} Double;

typedef struct {
    uint16_t index;
    UTF8* string;
} String;

typedef struct {
    uint8_t referenceKind;
    uint16_t referenceIndex;
} MethodHandle;

typedef struct {
    uint16_t descriptorIndex;
} MethodType;

typedef struct {
    uint16_t bootstrapMethodAttrIndex;
    uint16_t nameAndTypeIndex;
} InvokeDynamic;

typedef union {
    UTF8* utf8;
    MethodRef* methodRef;
    Class* class;
    NameAndType* nameAndType;
    Integer* integer;
    Float* f;
    Long* l;
    Double* d;
    String* string;
    MethodHandle* methodHandle;
    MethodType* methodType;
    InvokeDynamic* invokeDynamic;
} Constant;

typedef struct {
    uint8_t tag;
    Constant* constant;
} ConstantPoolInfo;

typedef struct {
    uint16_t size;
    ConstantPoolInfo** pool;
} ConstantPool;

ConstantPool* readConstantPool(const uint8_t** bytes);

#endif